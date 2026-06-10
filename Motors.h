#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include <Zumo32U4.h>

class Motors {
public:
  /**
   * Creates the Motors object and stores the encoder counts per centimeter.
   * @param countsForOneCm Encoder counts that are approximately equal to 1 cm.
   * @return Constructor has no return value.
   */
  Motors(uint16_t countsForOneCm = 75);

  /**
   * Prepares the motors and encoders for use.
   * @return void
   */
  void begin();

  /**
   * Stops both motors.
   * @return void
   */
  void stop();

  /**
   * Sets the speed of the left and right motor.
   * @param leftSpeed Speed of the left motor, from -400 to 400.
   * @param rightSpeed Speed of the right motor, from -400 to 400.
   * @return void
   */
  void setSpeeds(int16_t leftSpeed, int16_t rightSpeed);

  /**
   * Makes the Zumo drive straight forward.
   * @param speed Speed of both motors, from 0 to 400.
   * @return void
   */
  void driveForward(int16_t speed);

  /**
   * Makes the Zumo turn left in place.
   * @param speed Turn speed, from 0 to 400.
   * @return void
   */
  void turnLeft(int16_t speed);

  /**
   * Makes the Zumo turn right in place.
   * @param speed Turn speed, from 0 to 400.
   * @return void
   */
  void turnRight(int16_t speed);

  /**
   * Drives a fixed distance forward using the encoders.
   * @param distanceCm Distance in centimeters.
   * @param speed Speed while driving, from 0 to 400.
   * @return void
   */
  void driveDistanceCm(uint8_t distanceCm, int16_t speed);

private:
  Zumo32U4Motors zumoMotors;
  Zumo32U4Encoders encoders;

  uint16_t countsPerCm;

  /**
   * Keeps the motor speed between -400 and 400.
   * @param speed Speed that needs to be checked.
   * @return int16_t Safe speed between -400 and 400.
   */
  int16_t keepSpeedSafe(int16_t speed);

  /**
   * Resets the encoder counters to 0.
   * @return void
   */
  void resetEncoders();

  /**
   * Calculates how many encoder counts the Zumo has driven on average.
   * @return long Average absolute encoder counts from left and right.
   */
  long getAverageCounts();
};

#endif
