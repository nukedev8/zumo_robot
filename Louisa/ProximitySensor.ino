#include <Wire.h>
#include <Zumo32U4.h>
#include "functies.h"

AfstandsSensoren sensoren;

void setup(){
  sensoren.init();  //roept de void "init" aan uit de cpp file
}

void loop(){
  sensoren.aanKnop();  //roept de void "aanKnop" aan uit de cpp file
}
