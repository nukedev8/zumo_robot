#include "functies.h"

AfstandsSensoren::AfstandsSensoren():motoren_aan(false), objectVoorSensor(2), draaiSnelheid(180), vooruit(120)
{ //maakt een constructor aan die de bovenstaande waardes construeerd

}

AfstandsSensoren::~AfstandsSensoren(){
  motors.setSpeeds(0,0);  //deconstructor die ervoor zorgd dat de motoren stoppen als deze aangeroepen wordt
} 

void AfstandsSensoren::init(){
  proxSensors.initFrontSensor();  //roept de code voor de sensoren uit de library aan
}

void AfstandsSensoren::draaiRechts(){
  motors.setSpeeds(draaiSnelheid, -draaiSnelheid);  //zet de draaisnelheid van de rechter motor naar voren en die van de linker motor naar achter waardoor de zumo naar rechts draait
}

void AfstandsSensoren::draaiLinks(){
  motors.setSpeeds(-draaiSnelheid, draaiSnelheid);  //zet de draaisnelheid van de rechter motor naar achter en die van de linker motor naar voren waardoor de zumo naar links draait
}

void AfstandsSensoren::stop(){
  motors.setSpeeds(0,0);  //zet de snelheid van beide motoren naar 0 waardoor ze stoppen
}

void AfstandsSensoren::start(){
  motors.setSpeeds(vooruit, vooruit);  //zet de snelheid van beide motoren naar "vooruit", deze waarde is aan te passen in de .h file
}

void AfstandsSensoren::aanKnop(){
  proxSensors.read();
  uint8_t linksProxSensor = proxSensors.countsFrontWithLeftLeds();
  uint8_t rechtsProxSensor = proxSensors.countsFrontWithRightLeds(); 

 if (buttonA.isPressed()){
    motoren_aan = !motoren_aan;  
    /*als de A knop word ingedrukt veranderd de waarde van de bool "motoren_aan" deze word later gebruikt voor de if conditie om de motoren te laten draaien, 
    de knop word dus gebruikt om de motoren aan en uit te zetten, de delay zorgt ervoor dat de bool niet terug veranderd als de knop wat langer ingedrukt wordt*/
    delay (200);
  }  

 if (motoren_aan){
    ledYellow(1);  //zet het gele lampje aan als de bool "motoren_aan" true is waardoor te zien is dat de zumo aan het volgen is

    if (linksProxSensor > objectVoorSensor && rechtsProxSensor > objectVoorSensor){ 
      stop();  //roept de void "stop" op als de waarde van beide sensoren groter is dan de vooraf ingestelde waarde van "objectVoorSensor", hierdoor stopt de zumo als die te dicht bij een object komt
    }
    else if (linksProxSensor < rechtsProxSensor){
      draaiRechts();  //roept de void "draaiRechts" aan die de Zumo naar rechts laat draaien als de waarde van de rechter sensor groter is dan de waarde van de linker sensor
    }
    else if (linksProxSensor > objectVoorSensor && rechtsProxSensor > objectVoorSensor){ 
      stop();
    }
    else if (linksProxSensor > rechtsProxSensor){
      draaiLinks();  //roept de void "draaiLinks" aan die de Zumo naar links laat draaien als de waarde van de linker sensor groter is dan de waarde van de rechter sensor
    }
    else {
    start();  //als geen van de bovenstaande if statements waar zijn word de void "start" aangeroepen en rijd de Zumo recht vooruit
    }
  }

 else{
    ledYellow(0);  //als de bool "motoren_aan" niet true is staat de gele led uit en word de void "stop" aangetoepen waardoor de motoren stoppen
    stop();
  }
}
