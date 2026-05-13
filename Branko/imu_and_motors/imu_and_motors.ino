#include "Imu.h"
#include "Motors.h"
#include <Wire.h>

Motors motors;
Imu imu;
Zumo32U4ButtonA buttonA;

static void testMovement() {
    buttonA.waitForButton();
    delay(2000);

    for (int i = 0; i < 5; ++i) {
        motors.move(1000, 1000);
        delay(500);
        motors.move(-1000, -1000);
        delay(500);
    }
    Serial.println("Movement test finished");
}

static void testTurning() {
    buttonA.waitForButton();
    delay(2000);

    for (int i = 0; i < 5; ++i) {
        motors.move(-1000, 1000);
        delay(500);
        motors.move(1000, -1000);
        delay(500);
    }
    Serial.println("Turning test finished");
}

static void testImu() {
    Wire.begin();
    if (imu.init()) {
        while (true) {
            Serial.println("Failed to init IMU");
            delay(250);
        }
    }

    ledYellow(true);
    imu.calibrate();
    ledYellow(false);

    int32_t begin = millis();
    int32_t last_update = begin;
    while (true) {
        imu.update();
        int32_t now = millis();
        if (now - begin > 60000)
            break;
        else if (now - begin > 250) {
            if (buttonA.getSingleDebouncedPress())
                imu.reset();
            Serial.print(imu.getHeading());
            Serial.print(' ');
            Serial.print(imu.getPosition());
            Serial.print(' ');
            Serial.println(imu.getVelocity());
            last_update = now;
        }
    }
    Serial.println("IMU test finished");
}

void setup() {
    testImu();
}

void loop() {
    while (!Serial.available());
    String input = Serial.readStringUntil('\n');

    input.toLowerCase();
    switch (input[0]) {
    case 'w':
        motors.move(1000);
        break;
    case 's':
        motors.move(-1000);
        break;
    case 'a':
        motors.move(-500, 500);
        break;
    case 'd':
        motors.move(500, -500);
        break;
    }
}
