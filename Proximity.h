#ifndef promixity_h
#define promixity_h

#include <Arduino.h>
#include <Zumo32U4.h>

enum class ObjectDirection {
  None,
  Left,
  Right,
  Center
};

class Proximity {
public:
  /**
   * Maakt een Proximity object aan en bewaart de drempelwaarden.
   * @param detectValue Waarde vanaf waar een object wordt gezien.
   * @param closeValue Waarde vanaf waar een object dichtbij is.
   * @param centerDifference Maximaal verschil tussen links en rechts voor midden.
   * @return Constructor heeft geen returnwaarde.
   */
  Proximity(uint8_t detectValue = 2,
            uint8_t closeValue = 5,
            uint8_t centerDifference = 1);

  /**
   * Zet de proximity sensor aan de voorkant klaar voor gebruik.
   * @return void
   */
  void begin();

  /**
   * Leest de nieuwste proximity waarden uit.
   * @return void
   */
  void read();

  /**
   * Controleert of beide sensoren een object zien.
   * @return bool True als links en rechts boven de detectiewaarde zitten.
   */
  bool objectDetected();

  /**
   * Controleert of het object dichtbij genoeg is.
   * @return bool True als links en rechts boven de dichtbijwaarde zitten.
   */
  bool objectClose();

  /**
   * Bepaalt waar het object staat.
   * @return ObjectDirection Richting van het object: None, Left, Right of Center.
   */
  ObjectDirection getDirection();

  /**
   * Geeft de laatste linker proximity waarde terug.
   * @return uint8_t Linker proximity waarde.
   */
  uint8_t getLeftValue();

  /**
   * Geeft de laatste rechter proximity waarde terug.
   * @return uint8_t Rechter proximity waarde.
   */
  uint8_t getRightValue();

private:
  Zumo32U4ProximitySensors proxSensors;

  uint8_t leftValue;
  uint8_t rightValue;
  uint8_t detectThreshold;
  uint8_t closeThreshold;
  uint8_t centerMargin;
};

#endif
