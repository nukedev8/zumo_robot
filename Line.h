#ifndef LINE_H
#define LINE_H

#include <Arduino.h>
#include <Zumo32U4.h>

class Line {
public:
  static const uint8_t sensorCount = 5;

  /**
   * Creates the Line object and stores the sensor thresholds.
   * @param brownLineMinValue Lowest value that still counts as the brown line.
   * @param brownLineMaxValue Highest value that still counts as the brown line.
   * @param blackLineValue Value from which a black line is detected.
   * @param brownLineSensorsNeeded Number of sensors that must see the brown line.
   * @return Constructor has no return value.
   */
  Line(uint16_t brownLineMinValue = 80,
       uint16_t brownLineMaxValue = 120,
       uint16_t blackLineValue = 700,
       uint8_t brownLineSensorsNeeded = 4);

  /**
   * Prepares the five line sensors for use.
   * @return void
   */
  void begin();

  /**
   * Reads the newest values from the line sensors.
   * @return void
   */
  void read();

  /**
   * Checks if at least one sensor sees a black line.
   * @return bool True if a black line is detected, otherwise false.
   */
  bool seesBlackLine();

  /**
   * Checks if the robot has crossed the brown line.
   * @return bool True if enough sensors see the brown line.
   */
  bool crossedBrownLine();

  /**
   * Calculates where the black line is under the robot.
   * @return int Position from 0 left to 4000 right, 2000 is center.
   */
  int getLinePosition();

  /**
   * Returns the last measured value from one line sensor.
   * @param sensorNumber Sensor number, from 0 to 4.
   * @return uint16_t Sensor value, or 0 if the number does not exist.
   */
  uint16_t getValue(uint8_t sensorNumber);

private:
  Zumo32U4LineSensors lineSensors;
  uint16_t sensorValues[sensorCount];

  uint16_t brownLineMinThreshold;
  uint16_t brownLineMaxThreshold;
  uint16_t blackLineThreshold;
  uint8_t brownLineSensorCount;
};

#endif
