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
     * Maakt een Lijnsensor object aan.
     * @param m Referentie naar de motoren van de Zumo.
     * @param b Referentie naar knop A van de Zumo.
     * @return Constructor heeft geen returnwaarde.
     */
    Lijnsensor(Zumo32U4Motors& m, Zumo32U4ButtonA& b);

    /**
     * Zet de vijf lijnsensoren klaar voor gebruik.
     * @return void
     */
    void begin();

    /**
     * Kalibreert de lijnsensoren door de Zumo heen en weer te draaien.
     * @return void
     */
    void calibrate();

    /**
     * Leest de lijnsensoren uit en berekent de positie van de lijn.
     * @return void
     */
    void update();

    /**
     * Geeft de laatste waarden van de vijf lijnsensoren terug.
     * @return const uint16_t* Pointer naar de array met vijf sensorwaarden.
     */
    const uint16_t* getValues() const { return values; }

    /**
     * Geeft de fout ten opzichte van het midden van de lijn.
     * @return int16_t Negatief is links, positief is rechts, 0 is midden.
     */
    int16_t getError() const { return position - 2000; }

    /**
     * Controleert of de lijn kwijt is.
     * @return bool True als er geen lijn wordt gezien, anders false.
     */
    bool    lineLost() const { return position < 0; }

    /**
     * Geeft de huidige state van de robot terug.
     * @return LineState Huidige state van de state machine.
     */
    LineState getState() const { return state; }

    /**
     * Zet de robot in een nieuwe state.
     * @param s Nieuwe state waar de robot naartoe moet.
     * @return void
     */
    void      setState(LineState s);

    /**
     * Past de state machine aan op basis van de gedetecteerde kleur.
     * @param c Kleur die door de kleurdetectie is gevonden.
     * @return void
     */
    void      updateStateMachine(Color c);

    /**
     * Geeft hoe lang de robot al in de huidige state zit.
     * @return unsigned long Tijd in milliseconden sinds de laatste state-wissel.
     */
    unsigned long timeInState() const;

    /**
     * Controleert of de lijn net opnieuw is gevonden.
     * @return bool True als de lijn net is teruggevonden, anders false.
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
