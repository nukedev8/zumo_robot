#include "PowerManager.h"
#include "Buzzer.h"
#include <Zumo32U4.h>

Buzzer buzzer;         ///< Buzzer instantie voor geluidsuitvoer.
PowerManager power;    ///< PowerManager instantie voor stroombron detectie.

/**
 * @brief Arduino setup – wordt eenmalig uitgevoerd bij het opstarten.
 *
 * Speelt een lage toon (450 Hz) af als USB aangesloten is,
 * of een hoge toon (3000 Hz) als de batterij de stroombron is.
 */
void setup() {
    Serial.begin(9600);

    if (power.USBPowerPresent()) {
        delay(2000);

        if (power.USBPowerPresent()) {
            buzzer.playFrequency(450, 500, 8);
        }

        // TODO: motoren uitschakelen zolang USB aangesloten is,
        //       zodat de Zumo niet van de tafel rijdt.
        // motor.speed(0);
    }

    if (power.batteryPowerPresent()) {
        // TODO: vervang Serial door buzzer of LED.
        Serial.println("Battery connected");
        buzzer.playFrequency(3000, 1500, 8);
    }
}

/**
 * @brief Arduino loop – wordt herhaaldelijk uitgevoerd.
 *
 * Momenteel geen implementatie.
 */
void loop() {}