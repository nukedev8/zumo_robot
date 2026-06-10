#include <Wire.h>
#include <Zumo32U4.h>

#include "Line.h"
#include "Motors.h"
#include "Proximity.h"

enum RobotState {
  Waiting,
  DrivingUntilBrownLine,
  DrivingToMiddle,
  TurningUntilBlockFound,
  DrivingToBlock,
  PushingBlock,
  Done
};

const uint16_t brownLineMinThreshold = 80;
const uint16_t brownLineMaxThreshold = 120;
const uint16_t blackLineThreshold = 700;
const uint16_t countsPerCm = 75;

const int16_t searchBrownLineSpeed = 180;
const int16_t driveToMiddleSpeed = 220;
const int16_t turnSpeed = 140;
const int16_t driveToBlockSpeed = 180;
const int16_t pushSpeed = 260;

const uint8_t detectThreshold = 2;
const uint8_t closeThreshold = 5;
const uint8_t centerMargin = 1;

const uint8_t brownLineSensorsNeeded = 3;

const unsigned long printTimeMs = 300;
const unsigned long blockConfirmTimeMs = 150;
const unsigned long brownLineConfirmTimeMs = 0;
const unsigned long driveToMiddleTimeMs = 1400;
const unsigned long pushConfirmTimeMs = 700;
const unsigned long maxPushTimeMs = 6000;
const unsigned long pushOutsideCircleTimeMs = 450;

Line line(brownLineMinThreshold,
          brownLineMaxThreshold,
          blackLineThreshold,
          brownLineSensorsNeeded);
Motors motors(countsPerCm);
Proximity proximity(detectThreshold, closeThreshold, centerMargin);

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;

RobotState state = Waiting;
unsigned long stateStartTime = 0;
unsigned long lastPrintTime = 0;
unsigned long firstBrownLineSeenTime = 0;
unsigned long firstBlockSeenTime = 0;
unsigned long firstPushReadyTime = 0;
unsigned long firstCircleLineSeenTime = 0;

void changeState(RobotState newState);
void waitForStart();
void driveUntilBrownLine();
void driveToMiddle();
void turnUntilBlockFound();
void driveToBlock();
void pushBlock();
void waitWhenFinished();
bool brownLineConfirmed();
bool blockConfirmed();
bool blockSeenByBothSensors(uint8_t neededValue);
bool blockStraightInFront(ObjectDirection direction, uint8_t neededValue);
bool blockReadyToPush(ObjectDirection direction);
bool pushConfirmed(ObjectDirection direction);
void printLineValues();
void printProximityValues(ObjectDirection direction);
void printDirection(ObjectDirection direction);

/**
 * Starts the Arduino code and prepares all parts.
 * @return void
 */
void setup() {
  Wire.begin();
  Serial.begin(9600);

  line.begin();
  proximity.begin();
  motors.begin();

  changeState(Waiting);
  Serial.println("Zumo klaar. Zet hem bij het einddeel en druk op A.");
}

/**
 * Runs again and again and chooses what the Zumo should do.
 * @return void
 */
void loop() {
  if (buttonB.getSingleDebouncedPress()) {
    motors.stop();
    changeState(Waiting);
    Serial.println("Gestopt met knop B.");
  }

  switch (state) {
    case Waiting:
      waitForStart();
      break;

    case DrivingUntilBrownLine:
      driveUntilBrownLine();
      break;

    case DrivingToMiddle:
      driveToMiddle();
      break;

    case TurningUntilBlockFound:
      turnUntilBlockFound();
      break;

    case DrivingToBlock:
      driveToBlock();
      break;

    case PushingBlock:
      pushBlock();
      break;

    case Done:
      waitWhenFinished();
      break;
  }
}

/**
 * Changes the current robot state and turns on the correct LED.
 * @param newState New state the robot should go to.
 * @return void
 */
void changeState(RobotState newState) {
  state = newState;
  stateStartTime = millis();
  lastPrintTime = 0;
  firstBrownLineSeenTime = 0;
  firstBlockSeenTime = 0;
  firstPushReadyTime = 0;
  firstCircleLineSeenTime = 0;

  ledRed(false);
  ledYellow(false);
  ledGreen(false);

  if (state == Waiting) {
    ledYellow(true);
  }

  if (state == TurningUntilBlockFound || state == DrivingToBlock) {
    ledRed(true);
  }

  if (state == Done) {
    ledGreen(true);
  }
}

/**
 * Makes the robot wait until button A is pressed.
 * @return void
 */
void waitForStart() {
  motors.stop();

  if (buttonA.getSingleDebouncedPress()) {
    line.read();

    if (line.crossedBrownLine()) {
      Serial.println("Start: bruine lijn is al onder de robot.");
      changeState(DrivingToMiddle);
    } else {
      Serial.println("Start: rustig vooruit tot de bruine lijn.");
      changeState(DrivingUntilBrownLine);
    }
  }
}

/**
 * Drives straight forward until the brown line is detected.
 * @return void
 */
void driveUntilBrownLine() {
  line.read();

  if (millis() - lastPrintTime >= printTimeMs) {
    lastPrintTime = millis();
    printLineValues();
  }

  if (brownLineConfirmed()) {
    motors.stop();
    Serial.println("Bruine lijn gevonden.");
    changeState(DrivingToMiddle);
    return;
  }

  motors.driveForward(searchBrownLineSpeed);
}

/**
 * Drives from the brown line to the middle of the circle.
 * @return void
 */
void driveToMiddle() {
  if (lastPrintTime == 0) {
    Serial.println("Rijdt naar het midden van de cirkel.");
    lastPrintTime = millis();
  }

  if (millis() - stateStartTime >= driveToMiddleTimeMs) {
    motors.stop();
    Serial.println("Draait op zijn plek tot de kubus is gevonden.");
    changeState(TurningUntilBlockFound);
    return;
  }

  motors.driveForward(driveToMiddleSpeed);
}

/**
 * Turns in place until the cube is detected by both proximity sides.
 * @return void
 */
void turnUntilBlockFound() {
  proximity.read();
  ObjectDirection direction = proximity.getDirection();

  if (millis() - lastPrintTime >= printTimeMs) {
    lastPrintTime = millis();
    printProximityValues(direction);
  }

  if (blockConfirmed()) {
    motors.stop();
    Serial.println("Kubus gevonden. Naar de kubus toe rijden.");
    changeState(DrivingToBlock);
    return;
  }

  motors.turnRight(turnSpeed);
}

/**
 * Turns toward the cube or drives forward when it is in the center.
 * @return void
 */
void driveToBlock() {
  proximity.read();
  ObjectDirection direction = proximity.getDirection();
  bool objectIsBlock = blockConfirmed();
  bool blockInFront = blockStraightInFront(direction, detectThreshold);

  if (millis() - lastPrintTime >= printTimeMs) {
    lastPrintTime = millis();
    printProximityValues(direction);
  }

  if (objectIsBlock && pushConfirmed(direction)) {
    motors.stop();
    Serial.println("Kubus staat recht en dichtbij. Nu duwen.");
    changeState(PushingBlock);
    return;
  }

  if (direction == ObjectDirection::Left) {
    motors.turnLeft(turnSpeed);
  } else if (direction == ObjectDirection::Right) {
    motors.turnRight(turnSpeed);
  } else if (blockInFront) {
    motors.driveForward(driveToBlockSpeed);
  } else {
    motors.turnRight(turnSpeed);
  }
}

/**
 * Pushes the cube until it is outside the black circle line.
 * @return void
 */
void pushBlock() {
  if (lastPrintTime == 0) {
    Serial.println("Kubus recht vooruit duwen.");
    lastPrintTime = millis();
  }

  line.read();

  if (line.seesBlackLine()) {
    if (firstCircleLineSeenTime == 0) {
      firstCircleLineSeenTime = millis();
      Serial.println("Zwarte cirkellijn gezien. Nog kort doorduwen.");
    }
  }

  if (firstCircleLineSeenTime != 0) {
    if (millis() - firstCircleLineSeenTime >= pushOutsideCircleTimeMs) {
      motors.stop();
      Serial.println("Kubus is buiten de cirkel. Stop.");
      changeState(Done);
      return;
    }

    motors.driveForward(pushSpeed);
    return;
  }

  if (millis() - stateStartTime >= maxPushTimeMs) {
    motors.stop();
    Serial.println("Maximale duwtijd bereikt. Stop.");
    changeState(Done);
    return;
  }

  motors.driveForward(pushSpeed);
}

/**
 * Keeps the robot stopped when it is done.
 * @return void
 */
void waitWhenFinished() {
  motors.stop();

  if (buttonA.getSingleDebouncedPress()) {
    Serial.println("Nieuwe poging.");
    changeState(Waiting);
  }
}

/**
 * Checks if the brown line is visible long enough to count as crossed.
 * @return bool True if the brown line is confirmed, otherwise false.
 */
bool brownLineConfirmed() {
  if (line.crossedBrownLine()) {
    if (firstBrownLineSeenTime == 0) {
      firstBrownLineSeenTime = millis();
    }

    if (millis() - firstBrownLineSeenTime >= brownLineConfirmTimeMs) {
      return true;
    }
  } else {
    firstBrownLineSeenTime = 0;
  }

  return false;
}

/**
 * Checks if the object is visible long enough to count as the cube.
 * @return bool True if the cube is confirmed, otherwise false.
 */
bool blockConfirmed() {
  if (blockSeenByBothSensors(detectThreshold)) {
    if (firstBlockSeenTime == 0) {
      firstBlockSeenTime = millis();
    }

    if (millis() - firstBlockSeenTime >= blockConfirmTimeMs) {
      return true;
    }
  } else {
    firstBlockSeenTime = 0;
  }

  return false;
}

/**
 * Checks if both proximity sensors see the cube.
 * @param neededValue Minimum value needed on both sensors.
 * @return bool True if left and right both see the cube.
 */
bool blockSeenByBothSensors(uint8_t neededValue) {
  if (proximity.getLeftValue() >= neededValue &&
      proximity.getRightValue() >= neededValue) {
    return true;
  }

  return false;
}

/**
 * Checks if the cube is straight in front and seen by both sensors.
 * @param direction Direction in which the object is seen.
 * @param neededValue Minimum value needed on both sensors.
 * @return bool True if the cube is centered and seen by both sensors.
 */
bool blockStraightInFront(ObjectDirection direction, uint8_t neededValue) {
  if (direction == ObjectDirection::Center &&
      blockSeenByBothSensors(neededValue)) {
    return true;
  }

  return false;
}

/**
 * Checks if the cube is close and centered enough to start pushing.
 * @param direction Direction in which the object is seen.
 * @return bool True if the cube is close and straight in front of the Zumo.
 */
bool blockReadyToPush(ObjectDirection direction) {
  if (blockStraightInFront(direction, closeThreshold)) {
    return true;
  }

  return false;
}

/**
 * Checks if the cube stays ready long enough before pushing.
 * @param direction Direction in which the object is seen.
 * @return bool True if the cube is ready to push for long enough.
 */
bool pushConfirmed(ObjectDirection direction) {
  if (blockReadyToPush(direction)) {
    if (firstPushReadyTime == 0) {
      firstPushReadyTime = millis();
    }

    if (millis() - firstPushReadyTime >= pushConfirmTimeMs) {
      return true;
    }
  } else {
    firstPushReadyTime = 0;
  }

  return false;
}

/**
 * Prints the line sensor values to the Serial Monitor.
 * @return void
 */
void printLineValues() {
  Serial.print("Lijn: ");

  for (uint8_t i = 0; i < Line::sensorCount; i++) {
    Serial.print(line.getValue(i));

    if (i < Line::sensorCount - 1) {
      Serial.print(", ");
    }
  }

  Serial.print(" | zwart=");
  Serial.print(line.seesBlackLine());
  Serial.print(" | bruin=");
  Serial.println(line.crossedBrownLine());
}

/**
 * Prints the proximity values to the Serial Monitor.
 * @param direction Direction in which the object is seen.
 * @return void
 */
void printProximityValues(ObjectDirection direction) {
  Serial.print("Proximity L=");
  Serial.print(proximity.getLeftValue());
  Serial.print(" R=");
  Serial.print(proximity.getRightValue());
  Serial.print(" richting=");
  printDirection(direction);
  Serial.print(" beide=");
  Serial.print(blockSeenByBothSensors(detectThreshold));
  Serial.print(" dichtbij=");
  Serial.println(proximity.objectClose());
}

/**
 * Prints the ObjectDirection as text to the Serial Monitor.
 * @param direction Direction that should be printed.
 * @return void
 */
void printDirection(ObjectDirection direction) {
  if (direction == ObjectDirection::Left) {
    Serial.print("links");
  } else if (direction == ObjectDirection::Right) {
    Serial.print("rechts");
  } else if (direction == ObjectDirection::Center) {
    Serial.print("voor");
  } else {
    Serial.print("geen");
  }
}
