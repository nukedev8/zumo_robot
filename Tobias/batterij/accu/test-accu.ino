#include "Battery.h"

Battery bat;

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("=== Battery test ===");
}

void loop() {
    Serial.print("Spanning: ");
    Serial.print(bat.readMillivolts());
    Serial.println(" mV");

    if (bat.isLow()) {
        Serial.println("Batterij is bijna leeg!");
    } else {
        Serial.println("Batterij is OK");
    }

    Serial.println("---");
    delay(10000);
}