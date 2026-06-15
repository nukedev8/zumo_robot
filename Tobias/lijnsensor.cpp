#include <Arduino.h>
#include "Lijnsensor.h"
#include "Kleurdetectie.h"


/**
* Lijnsensor.cpp
* implementatie van de Lijnsensor.
*/

static const uint16_t LINE_THRESHOLD = 100;
static const uint8_t  HISTORY_SIZE   = 7;

Lijnsensor::Lijnsensor(Zumo32U4Motors& m, Zumo32U4ButtonA& b)
    : motors(m), buttonA(b),
      historyIndex(0), position(-1),
      state(LineState::Idle), stateStartTime(0),
      prevLineLost(false), lineRecoveredEdge(false)
{
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        values[i] = 0;
        for (uint8_t j = 0; j < HISTORY_SIZE; j++) history[i][j] = 0;
    }
}

/**
* initialiseert de IR-sensoren zodat ze alle 5 worden gebruikt.
*/
void Lijnsensor::begin() {
    lineSensors.initFiveSensors();
}

/**
* als op A wordt gedrukt draait ie van links naar rechts over de baan.
* dit zodat er gekalibreerde data uitkomt (is veel accurater).
*/
void Lijnsensor::calibrate() {
    buttonA.waitForButton();
    for (uint16_t i = 0; i < 120; i++) {
        if (i > 30 && i <= 90) motors.setSpeeds(-200, 200);
        else                   motors.setSpeeds( 200,-200);
        lineSensors.calibrate();
    }
    motors.setSpeeds(0, 0);
}

/**
* leest sensoren uit, zorgt voor gemiddelde en berekent de positie.
*
* leest ruwe data uit.
* zet dat in de "geschiedenis".
* berekent gemiddelde per sensor.
* bepaalt de lijnpositie met dat gemiddelde.
*
*/
void Lijnsensor::update() {

    uint16_t raw[NUM_SENSORS];
    lineSensors.readLine(raw);
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        history[i][historyIndex] = raw[i];
        uint32_t sum = 0;
        for (uint8_t j = 0; j < HISTORY_SIZE; j++) sum += history[i][j];
        values[i] = sum / HISTORY_SIZE;
    }
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    
    uint32_t wsum = 0, total = 0;
    bool anyDark = false;
    for (uint8_t i = 0; i < NUM_SENSORS; i++) {
        if (values[i] > LINE_THRESHOLD) {
            anyDark = true;
            wsum  += (uint32_t)values[i] * (i * 1000);
            total += values[i];
        }
    }
    position = (!anyDark || total == 0) ? -1 : (wsum / total);
    
    bool nowLost = lineLost();
    lineRecoveredEdge = (!nowLost && prevLineLost);
    prevLineLost = nowLost;
}
/**
* reset de timer bij verandering.
*/
void Lijnsensor::setState(LineState s) {
    if (s != state) {
        state = s;
        stateStartTime = millis();
    }
}
/**
* tijd in ms sinds laatste verandering.
*/
unsigned long Lijnsensor::timeInState() const {
    return millis() - stateStartTime;
}

/**
* geeft true terug bij het vinden van de lijn.
* wordt daarna gereset.
*/
bool Lijnsensor::sawLineRecovered() {
    bool e = lineRecoveredEdge;
    lineRecoveredEdge = false;
    return e;
}

/**
* Verwerkt state-overgangen op basis van de kleur
*
* main regelt de states voor het vinden van het blok.
*/
void Lijnsensor::updateStateMachine(Color c) {
    switch (state) {
        case LineState::FollowLine:
            if (c == Color::Green) setState(LineState::SlowZone);
            else if (c == Color::Brown && timeInState() > 1000)
                setState(LineState::EndZoneApproach);
            else if (lineLost() && timeInState() > 100)
                setState(LineState::LineLost);
            break;
        
        case LineState::SlowZone:
            if (c != Color::Green) setState(LineState::FollowLine);
            break;
        
        case LineState::LineLost:
            if (sawLineRecovered()) setState(LineState::FollowLine);
            else if (timeInState() > 3500) setState(LineState::FollowLine);
            break;
        
        default:
            break;  // Idle, EndZoneApproach, SearchingBlock, Finished: main bestuurt
    }
}