#include <Arduino.h>
#include "Lijnsensor.h"
#include "Kleurdetectie.h"


/**
* Lijnsensor.cpp
* implementatie van de Lijnsensor.
*/

static const uint16_t LINE_THRESHOLD = 100;
static const uint8_t  HISTORY_SIZE   = 7;

/**
 * Maakt een Lijnsensor object aan en zet de beginwaarden klaar.
 * @param m Referentie naar de motoren van de Zumo.
 * @param b Referentie naar knop A van de Zumo.
 * @return Constructor heeft geen returnwaarde.
 */
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
 * Zet de vijf lijnsensoren klaar voor gebruik.
 * @return void
 */
void Lijnsensor::begin() {
    lineSensors.initFiveSensors();
}

/**
 * Kalibreert de lijnsensoren door de Zumo heen en weer te draaien.
 * @return void
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
 * Leest de lijnsensoren uit en berekent de positie van de lijn.
 * @return void
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
 * Zet de robot in een nieuwe state en reset de state-timer.
 * @param s Nieuwe state waar de robot naartoe moet.
 * @return void
 */
void Lijnsensor::setState(LineState s) {
    if (s != state) {
        state = s;
        stateStartTime = millis();
    }
}
/**
 * Geeft hoe lang de robot al in de huidige state zit.
 * @return unsigned long Tijd in milliseconden sinds de laatste state-wissel.
 */
unsigned long Lijnsensor::timeInState() const {
    return millis() - stateStartTime;
}

/**
 * Controleert of de lijn net opnieuw is gevonden.
 * @return bool True als de lijn net is teruggevonden, anders false.
 */
bool Lijnsensor::sawLineRecovered() {
    bool e = lineRecoveredEdge;
    lineRecoveredEdge = false;
    return e;
}

/**
 * Past de state machine aan op basis van de gedetecteerde kleur.
 * @param c Kleur die door de kleurdetectie is gevonden.
 * @return void
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
