#include "buttons.h"

/*
 * Creates a button object and stores which Zumo button it controls.
 * @param buttonMode Chooses Button A, Button B, or Button C.
 * Constructor, no return.
 */
Button::Button(Mode buttonMode)
  : mode(buttonMode), previousPressed(false) {}

/*
 * Prepares the button object.
 * Return type: void.
 */
void Button::begin() {
}

/*
 * Checks if the button is being pressed right now.
 * @return bool, true when pressed, false when not pressed.
 */
bool Button::isPressed() {
  return readButton();
}

/*
 * Checks for one new button click and filters out button bounce.
 * @return bool, true once when a new click starts, false otherwise.
 */
bool Button::isClicked() {
  bool currentPressed = isPressed();

  if (currentPressed && !previousPressed) {
    delay(25);

    if (isPressed()) {
      previousPressed = currentPressed;
      return true;
    }
  }

  previousPressed = currentPressed;
  return false;
}

/*
 * Waits until the button is pressed and released.
 * Return type: void.
 */
void Button::waitForPress() {
  while (!isPressed()) {
  }

  delay(25);

  while (isPressed()) {
  }

  delay(25);
}

/*
 * Reads the selected Zumo button.
 * @return bool, true when the selected button is pressed, false when not pressed.
 */
bool Button::readButton() {
  if (mode == Mode::ButtonA) {
    return buttonA.isPressed();
  }

  if (mode == Mode::ButtonB) {
    return buttonB.isPressed();
  }

  return buttonC.isPressed();
}
