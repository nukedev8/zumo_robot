#ifndef BUTTONS_H
#define BUTTONS_H

#include <Zumo32U4.h>

/**
 * Reads one physical Zumo button.
 */
class Button {
public:
  /**
   * Selects how the button pin should be read.
   */
  enum class Mode {
    // Zumo buttons
    ButtonA,
    ButtonB,
    ButtonC
  };

  /**
   * Creates a button object.
   * @param buttonMode Chooses Button A, Button B, or Button C.
   * Constructor, no return.
   */
  Button(Mode buttonMode);

  /**
   * Configures the button pin as an input.
   * Return type: void.
   */
  void begin();

  /**
   * Checks whether the button is currently held down.
   * @return bool, true when pressed, false when not pressed.
   */
  bool isPressed();

  /**
   * Checks for one new debounced button click.
   * @return bool, true once for a new click, false if there is no new click.
   */
  bool isClicked();

  /**
   * Waits until the button is pressed and released.
   * Return type: void.
   */
  void waitForPress();

private:
  Mode mode;
  bool previousPressed;
  Zumo32U4ButtonA buttonA;
  Zumo32U4ButtonB buttonB;
  Zumo32U4ButtonC buttonC;

  /**
   * Reads the selected Zumo button.
   * @return bool, true when the selected button is pressed, false when not pressed.
   */
  bool readButton();
};

#endif
