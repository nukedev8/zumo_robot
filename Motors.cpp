#include "Motors.h"

/**
 * Creates the Motors object and stores the encoder counts per centimeter.
 * @param countsForOneCm Encoder counts that are approximately equal to 1 cm.
 * @return Constructor has no return value.
 */
Motors::Motors(uint16_t countsForOneCm) {
  countsPerCm = countsForOneCm;
}

/**
 * Prepares the encoders and stops the motors.
 * @return void
 */
void Motors::begin() {
  resetEncoders();
  stop();
}

/**
 * Stops both motors.
 * @return void
 */
void Motors::stop() {
  zumoMotors.setSpeeds(0, 0);
}

/**
 * Sets the speed of the left and right motor.
 * @param leftSpeed Speed of the left motor, from -400 to 400.
 * @param rightSpeed Speed of the right motor, from -400 to 400.
 * @return void
 */
void Motors::setSpeeds(int16_t leftSpeed, int16_t rightSpeed) {
  leftSpeed = keepSpeedSafe(leftSpeed);
  rightSpeed = keepSpeedSafe(rightSpeed);

  zumoMotors.setSpeeds(leftSpeed, rightSpeed);
}

/**
 * Makes the Zumo drive straight forward.
 * @param speed Speed of both motors, from 0 to 400.
 * @return void
 */
void Motors::driveForward(int16_t speed) {
  speed = keepSpeedSafe(speed);

  if (speed < 0) {
    speed = -speed;
  }

  zumoMotors.setSpeeds(speed, speed);
}

/**
 * Makes the Zumo turn left in place.
 * @param speed Turn speed, from 0 to 400.
 * @return void
 */
void Motors::turnLeft(int16_t speed) {
  speed = keepSpeedSafe(speed);

  if (speed < 0) {
    speed = -speed;
  }

  zumoMotors.setSpeeds(-speed, speed);
}

/**
 * Makes the Zumo turn right in place.
 * @param speed Turn speed, from 0 to 400.
 * @return void
 */
void Motors::turnRight(int16_t speed) {
  speed = keepSpeedSafe(speed);

  if (speed < 0) {
    speed = -speed;
  }

  zumoMotors.setSpeeds(speed, -speed);
}

/**
 * Drives a fixed distance forward using the encoders.
 * @param distanceCm Distance in centimeters.
 * @param speed Speed while driving, from 0 to 400.
 * @return void
 */
void Motors::driveDistanceCm(uint8_t distanceCm, int16_t speed) {
  long targetCounts = (long)distanceCm * countsPerCm;
  unsigned long startTime = millis();
  unsigned long maxDriveTimeMs = (unsigned long)distanceCm * 120 + 1200;

  speed = keepSpeedSafe(speed);

  if (speed < 0) {
    speed = -speed;
  }

  resetEncoders();

  while (getAverageCounts() < targetCounts &&
         millis() - startTime < maxDriveTimeMs) {
    long leftCounts = encoders.getCountsLeft();
    long rightCounts = encoders.getCountsRight();

    if (leftCounts < 0) {
      leftCounts = -leftCounts;
    }

    if (rightCounts < 0) {
      rightCounts = -rightCounts;
    }

    int16_t correction = (leftCounts - rightCounts) / 4;

    setSpeeds(speed - correction, speed + correction);
    delay(5);
  }

  stop();
}

/**
 * Keeps the motor speed between -400 and 400.
 * @param speed Speed that needs to be checked.
 * @return int16_t Safe speed between -400 and 400.
 */
int16_t Motors::keepSpeedSafe(int16_t speed) {
  if (speed > 400) {
    return 400;
  }

  if (speed < -400) {
    return -400;
  }

  return speed;
}

/**
 * Resets the encoder counters to 0.
 * @return void
 */
void Motors::resetEncoders() {
  encoders.getCountsAndResetLeft();
  encoders.getCountsAndResetRight();
}

/**
 * Calculates how many encoder counts the Zumo has driven on average.
 * @return long Average absolute encoder counts from left and right.
 */
long Motors::getAverageCounts() {
  long leftCounts = encoders.getCountsLeft();
  long rightCounts = encoders.getCountsRight();

  if (leftCounts < 0) {
    leftCounts = -leftCounts;
  }

  if (rightCounts < 0) {
    rightCounts = -rightCounts;
  }

  return (leftCounts + rightCounts) / 2;
}
  