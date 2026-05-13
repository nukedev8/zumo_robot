#include <Wire.h>
#include <Zumo32U4.h>
#include "buttons.h"
#include "irpulses.h"

Zumo32U4Motors motors;
Button buttonA(Button::Mode::ButtonA);
Button buttonB(Button::Mode::ButtonB);
Button buttonC(Button::Mode::ButtonC);
IRPulses irPulses;

const uint8_t BUTTON_COUNT = 3;
Button *const BUTTONS[BUTTON_COUNT] = {&buttonA, &buttonB, &buttonC};
const char BUTTON_NAMES[BUTTON_COUNT] = {'A', 'B', 'C'};

enum class RobotMode {
  Stopped,
  Paused,
  Racing
};

struct InputSnapshot {
  bool buttonPressed[BUTTON_COUNT];
  uint8_t irLeft;
  uint8_t irRight;
  bool objectFront;
};

RobotMode robotMode = RobotMode::Stopped;
const uint8_t DRIVE_SPEED = 120;
const uint8_t FRONT_OBJECT_THRESHOLD = 6;
const unsigned long INPUT_PRINT_INTERVAL_MS = 500;

void beginButtons();
void handleButtons();
void driveForward();
void stopMotors();
void setRobotMode(RobotMode newMode);
void runRaceMode();
bool stopIfObjectInFront();
InputSnapshot readInputs();
void printInputMonitor();
void printButtonInputs(const InputSnapshot &inputs);
void printIrInputs(const InputSnapshot &inputs);
void printRobotMode();
void sendMessage(const __FlashStringHelper *message);

/**
 * Initializes serial, motors, buttons, and IR pulses.
 * Return type: void.
 */
void setup() {
  Serial.begin(9600);
  beginButtons();
  irPulses.begin();

  stopMotors();
}

/**
 * Main Arduino loop that checks commands, buttons, and robot state.
 * Return type: void.
 */
void loop() {
  handleButtons();
  printInputMonitor();

  if (robotMode == RobotMode::Racing) {
    runRaceMode();
  } else {
    stopMotors();
  }
}

/**
 * Starts all button objects from the fixed button array.
 * Return type: void.
 */
void beginButtons() {
  for (uint8_t index = 0; index < BUTTON_COUNT; index++) {
    BUTTONS[index]->begin();
  }
}

/**
 * Handles Button A, Button B, and Button C actions.
 * Return type: void.
 */
void handleButtons() {
  if (buttonA.isClicked()) {
    
    setRobotMode(RobotMode::Racing);
  }

  if (buttonB.isClicked()) {
    setRobotMode(RobotMode::Stopped);
  }

  if (buttonC.isClicked()) {
    if (robotMode == RobotMode::Racing) {
      setRobotMode(RobotMode::Paused);
    } else if (robotMode == RobotMode::Paused) {
      setRobotMode(RobotMode::Racing);
    }
  }
}

void driveForward() {
  motors.setSpeeds(DRIVE_SPEED, DRIVE_SPEED);
}

void stopMotors() {
  motors.setSpeeds(0, 0);
}


/**
 * Changes the robot mode and sends a status message.
 * @param newMode Mode the robot should switch to.
 * Return type: void.
 */
void setRobotMode(RobotMode newMode) {
  if (robotMode == newMode) {
    return;
  }

  RobotMode oldMode = robotMode;
  robotMode = newMode;

  if (robotMode != RobotMode::Racing) {
    stopMotors();
  }

  if (robotMode == RobotMode::Racing) {
    if (oldMode == RobotMode::Paused) {
      sendMessage(F("Race resumed"));
    } else {
      sendMessage(F("Race started"));
    }
  } else if (robotMode == RobotMode::Paused) {
    sendMessage(F("Race paused"));
  } else if (robotMode == RobotMode::Stopped) {
    sendMessage(F("Race stopped"));
  }
}

void runRaceMode() {
  if (stopIfObjectInFront()) {
    return;
  }

  driveForward();
}

bool stopIfObjectInFront() {
  if (!irPulses.objectInFront(FRONT_OBJECT_THRESHOLD)) {
    return false;
  }

  robotMode = RobotMode::Stopped;
  stopMotors();
  sendMessage(F("Object front"));
  sendMessage(F("Race stopped"));
  return true;
}

/**
 * Reads all button and IR input values into one struct.
 * @return InputSnapshot - current values from the buttons and front IR sensor.
 */
InputSnapshot readInputs() {
  InputSnapshot inputs = {};

  for (uint8_t index = 0; index < BUTTON_COUNT; index++) {
    inputs.buttonPressed[index] = BUTTONS[index]->isPressed();
  }

  irPulses.read();
  inputs.irLeft = irPulses.frontLeftCount();
  inputs.irRight = irPulses.frontRightCount();
  inputs.objectFront = inputs.irLeft >= FRONT_OBJECT_THRESHOLD ||
                       inputs.irRight >= FRONT_OBJECT_THRESHOLD;

  return inputs;
}

/**
 * Prints button and IR input values to the Serial Monitor.
 * Return type: void.
 */
void printInputMonitor() {
  static unsigned long lastPrintTime = 0;

  if (millis() - lastPrintTime < INPUT_PRINT_INTERVAL_MS) {
    return;
  }

  lastPrintTime = millis();
  InputSnapshot inputs = readInputs();

  Serial.print(F("Mode="));
  printRobotMode();
  printButtonInputs(inputs);
  printIrInputs(inputs);
  Serial.println();
}

/**
 * Prints all button values from the input snapshot.
 * @param inputs InputSnapshot created by readInputs().
 * Return type: void.
 */
void printButtonInputs(const InputSnapshot &inputs) {
  for (uint8_t index = 0; index < BUTTON_COUNT; index++) {
    Serial.print(F(" | "));
    Serial.print(BUTTON_NAMES[index]);
    Serial.print(F("="));
    Serial.print(inputs.buttonPressed[index]);
  }
}

/**
 * Prints the IR values from the input snapshot.
 * @param inputs InputSnapshot created by readInputs().
 * Return type: void.
 */
void printIrInputs(const InputSnapshot &inputs) {
  Serial.print(F(" | IR_L="));
  Serial.print(inputs.irLeft);

  Serial.print(F(" | IR_R="));
  Serial.print(inputs.irRight);

  Serial.print(F(" | Object="));
  Serial.print(inputs.objectFront);
}

/**
 * Prints the current robot mode to the Serial Monitor.
 * Return type: void.
 */
void printRobotMode() {
  if (robotMode == RobotMode::Racing) {
    Serial.print(F("Racing"));
  } else if (robotMode == RobotMode::Paused) {
    Serial.print(F("Paused"));
  } else {
    Serial.print(F("Stopped"));
  }
}

/**
 * Sends one status message to the USB Serial Monitor.
 * @param message Text stored in flash memory using the F() macro.
 * Return type: void.
 */
void sendMessage(const __FlashStringHelper *message) {
  Serial.println(message);
}
