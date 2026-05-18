#include <Arduino.h>
#include "LineSensor.h"

LineSensorArray array;

void setup() {
    Serial.begin(9600);
    array.init();
    delay(1000);
    Serial.println("=== LineSensor test ===");
}

void loop() {
    uint16_t values[5];
    array.readAll(values);

    Serial.print("DN1:"); Serial.print(values[0]);
    Serial.print(" DN2:"); Serial.print(values[1]);
    Serial.print(" DN3:"); Serial.print(values[2]);
    Serial.print(" DN4:"); Serial.print(values[3]);
    Serial.print(" DN5:"); Serial.println(values[4]);

    delay(300);
}