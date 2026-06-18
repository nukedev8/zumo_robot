#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>
#include "Lijnsensor.h"
#include "Kleurdetectie.h"
#include "Proximity.h"

Zumo32U4Motors  motors;
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;

Lijnsensor    linePos(motors, buttonA);
Kleurdetectie color;
Proximity     proximity(2, 5, 1);

bool running = false;

static const uint16_t MAX_SPEED_BLACK = 350;
static const uint16_t MAX_SPEED_GREEN = 200;

static int16_t lastError = 0;

unsigned long zoekblok = 0;
const unsigned long rijd_naar_midden = 1400;
const unsigned long duw_over = 450;
unsigned long pushSawLineTime = 0;

unsigned long firstBlockSeenTime = 0;
const unsigned long BLOCK_CONFIRM_MS = 150;
bool blockFound = false;

/**
 * Zet de robot aan of uit met knop B.
 * @return void
 */
void toggleRunning() {
    running = !running;
    motors.setSpeeds(0, 0);
    if (running) {
        linePos.setState(LineState::FollowLine);
        lastError = 0;
        blockFound = false;
        firstBlockSeenTime = 0;
    } else {
        linePos.setState(LineState::Idle);
    }
}

/**
 * Berekent de motorsnelheden zodat de Zumo de lijn blijft volgen.
 * @param maxSpeed Maximale snelheid die de motoren mogen krijgen.
 * @return void
 */
void PID_controller(uint16_t maxSpeed) {
    int16_t error = linePos.getError();
    if (linePos.lineLost()) error = lastError;
    
    int16_t speedDifference = error / 2 + 7 * (error - lastError);
    lastError = error;

    int16_t leftSpeed  = (int16_t)maxSpeed + speedDifference;
    int16_t rightSpeed = (int16_t)maxSpeed - speedDifference;

    leftSpeed  = constrain(leftSpeed,  -(int16_t)maxSpeed, (int16_t)maxSpeed);
    rightSpeed = constrain(rightSpeed, -(int16_t)maxSpeed, (int16_t)maxSpeed);

    motors.setSpeeds(leftSpeed, rightSpeed);
}

/**
 * Start de onderdelen van de Zumo en wacht op knop A.
 * @return void
 */
void setup() {
    Wire.begin();
    linePos.begin();
    proximity.begin();

    linePos.calibrate();
    buttonA.waitForButton();

    running = true;
    linePos.setState(LineState::FollowLine);
}

/**
 * Hoofdprogramma dat steeds opnieuw wordt uitgevoerd.
 * @return void
 */
void loop() {
    linePos.update();
    color.update(linePos.getValues());

    if (buttonB.getSingleDebouncedPress()) toggleRunning();

    if (buttonC.getSingleDebouncedPress()) {
        linePos.setState(LineState::SearchingBlock);
        pushSawLineTime = 0;
        blockFound = false;
        firstBlockSeenTime = 0;
    }

    if (!running) {
        motors.setSpeeds(0, 0);
        return;
    }

    // State machine alleen updaten tijdens rij-fases
    LineState s = linePos.getState();
    if (s == LineState::FollowLine || 
        s == LineState::SlowZone || 
        s == LineState::LineLost) {
        linePos.updateStateMachine(color.getColor());
    }

    switch (linePos.getState()) {
        case LineState::Idle:
            motors.setSpeeds(0, 0);
            break;

        case LineState::FollowLine:
            PID_controller(MAX_SPEED_BLACK);
            break;

        case LineState::SlowZone:
            PID_controller(MAX_SPEED_GREEN);
            break;

        case LineState::LineLost: {
            unsigned long t = linePos.timeInState();
            if (t < 400)        motors.setSpeeds(-140, -140);
            else if (t < 1200)  motors.setSpeeds( 120, -120);
            else if (t < 2400)  motors.setSpeeds(-120,  120);
            else if (t < 3200)  motors.setSpeeds( 120, -120);
            else                motors.setSpeeds(0, 0);
            break;
        }

        case LineState::EndZoneApproach: {
            if (zoekblok == 0) zoekblok = millis();
            if (millis() - zoekblok < rijd_naar_midden) {
                motors.setSpeeds(220, 200);
            } else {
                motors.setSpeeds(0, 0);
                zoekblok = 0;
                pushSawLineTime = 0;
                blockFound = false;
                firstBlockSeenTime = 0;
                linePos.setState(LineState::SearchingBlock);
            }
            break;
        }

        case LineState::SearchingBlock: {
            proximity.read();
            ObjectDirection dir = proximity.getDirection();

            bool bothSee = (proximity.getLeftValue() >= 2 && 
                            proximity.getRightValue() >= 2);

            if (!blockFound) {
                if (bothSee) {
                    if (firstBlockSeenTime == 0) firstBlockSeenTime = millis();
                    if (millis() - firstBlockSeenTime >= BLOCK_CONFIRM_MS) {
                        blockFound = true;
                        motors.setSpeeds(0, 0);
                        break;
                    }
                } else {
                    firstBlockSeenTime = 0;
                }
                motors.setSpeeds(160, -160);
                break;
            }

            bool closeEnough = proximity.objectClose();

            if (closeEnough && dir == ObjectDirection::Center) {
                if (linePos.getValues()[0] >= 700 ||
                    linePos.getValues()[2] >= 700 ||
                    linePos.getValues()[4] >= 700) {
                    if (pushSawLineTime == 0) pushSawLineTime = millis();
                    if (millis() - pushSawLineTime >= duw_over) {
                        motors.setSpeeds(0, 0);
                        linePos.setState(LineState::Finished);
                        break;
                    }
                }
                motors.setSpeeds(250, 250);
                break;
            }

            if (dir == ObjectDirection::Left) {
                motors.setSpeeds(-160, 160);
            } else if (dir == ObjectDirection::Right) {
                motors.setSpeeds(160, -160);
            } else if (dir == ObjectDirection::Center) {
                motors.setSpeeds(200, 200);
            } else {
                blockFound = false;
                firstBlockSeenTime = 0;
                motors.setSpeeds(150, -150);
            }
            break;
        }

        case LineState::Finished:
            motors.setSpeeds(0, 0);
            break;

        default:
            motors.setSpeeds(0, 0);
            break;
    }
}
