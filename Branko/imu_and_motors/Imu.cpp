#include "Imu.h"
#include "ints.h"
#include <Wire.h>

int Imu::init() {
    if (!inner_imu.init())
        return 1;

    inner_imu.enableDefault();
    return 0;
}

void Imu::calibrate() {
    Vec<i32> gyro_total{};
    Vec<i32> acc_total{};
    i32 calibration_cycles = 4096;

    for (i32 i = millis(); i < calibration_cycles; ++i) {
        if (i % 64 == 0) {
            Serial.print("Calibrating IMU... (");
            Serial.print(i);
            Serial.print('/');
            Serial.print(calibration_cycles);
            Serial.println(')');
        }
        inner_imu.readGyro();
        inner_imu.readAcc();
        gyro_total += Vec<i32>(inner_imu.g);
        acc_total += Vec<i32>(inner_imu.a);
    }

    gyro_offset =
        (gyro_total.cast<double>() / calibration_cycles).cast<float>();
    acc_offset = (acc_total.cast<double>() / calibration_cycles).cast<float>();
}

void Imu::update() {
    inner_imu.readAcc();
    inner_imu.readGyro();

    i32 current_time = micros();
    i32 dt = current_time - last_update;
    last_update = current_time;

    Vec<float> gyro_reading = Vec<float>(inner_imu.g) - gyro_offset;
    Vec<float> acc_reading = Vec<float>(inner_imu.a) - acc_offset;

    heading += gyro_reading * dt / 1000000 * (8.75 / 1000);
    vel += acc_reading * dt / 16384 / 1000000 * 9.80665;
    pos += vel * dt / 1000000;
}

Vec<float> Imu::getHeading() const {
    return heading;
}

Vec<float> Imu::getVelocity() const {
    return vel;
}

Vec<float> Imu::getPosition() const {
    return pos;
}

void Imu::reset() {
    pos = {};
    vel = {};
    heading = {};
}
