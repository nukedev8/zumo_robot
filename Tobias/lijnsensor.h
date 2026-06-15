#ifndef LIJNSENSOR_H
#define LIJNSENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <Zumo32U4.h>


/**
* Lijnsensor.h
* Klasse voor het uitlezen van de lijnsensoren en het bijhouden van de state machine

*/

/**
* Wat de zumo allemaal doet.
*/
enum class LineState {
    Idle,           ///< stilstaan, wachten op A.
    FollowLine,     ///< lijnvolgen met hulp van de PD.
    SlowZone,       ///< langzaam rijden op de groene lijn.
    LineLost,       ///< lijn is kwijt, gaat hem hier zoeken.
    EndZoneApproach,///< de bruine lijn gezien, rij naar het midden van de cirkel
    SearchingBlock, ///< ga het blok zoeken met de proximity sensors
    Finished        ///< het blok is uit de cirkel geduwd.
};

enum class Color;

class Lijnsensor {
public:
    ///< aantal IR-sensoren die de zumo heeft.
    static const uint8_t NUM_SENSORS = 5;

    /**
    * m reference naar de motors.
    * b reference naar knop A.
    */
    Lijnsensor(Zumo32U4Motors& m, Zumo32U4ButtonA& b);

    /**
    * initialiseert de sensors.
    * aanroepen in setup() (doen anders niks).
    */
    void begin();

    /**
    * calibreert de sensor op zwart en wit als je 1 keer op A drukt.
    */
    void calibrate();

    /**
    * leest de sensoren uit, en berekent het gemiddelde.
    */
    void update();

    /**
    * pointer naar de gemiddelde sensorwaarde.
    * return pointer naar array van de 5 sensoren.
    * return waarde van -2000 tot +2000, meest linker tot meest rechter.
    * 0 is dan het midden.
    */
    const uint16_t* getValues() const { return values; }
    int16_t getError() const { return position - 2000; }
    bool    lineLost() const { return position < 0; }

    /**
    * de huidige stand van de state machine.
    * returnt de huidige state.
    */
    LineState getState() const { return state; }

    void      setState(LineState s);

    /**
    * update de state machine op basis van de kleur.
    */
    void      updateStateMachine(Color c);

    /**
    * hoeveel miliseconden de zumo in de huidige state zit.
    */
    unsigned long timeInState() const;

    /**
    * return true als de lijn is teruggevonden, anders false.
    * wordt gereset als de lijn is teruggevonden.
    */
    bool sawLineRecovered();

private:
    /// library van de IR-sensors.
    Zumo32U4LineSensors lineSensors;

    /// reference naar de motors.
    Zumo32U4Motors&     motors;

    /// reference naar knop A.
    Zumo32U4ButtonA&    buttonA;

    /// gemiddelde waarde per sensor.
    uint16_t values[NUM_SENSORS];

    /// hoe groot het geheugen is (om spikes tegen te gaan)
    uint16_t history[NUM_SENSORS][7];

    /// index daarvan.
    uint8_t  historyIndex;

    /// lijnpositie
    int16_t  position;

    /// Huidige state van de state machine.
    LineState     state;
    unsigned long stateStartTime;
    bool          prevLineLost;
    bool          lineRecoveredEdge;
};

#endif