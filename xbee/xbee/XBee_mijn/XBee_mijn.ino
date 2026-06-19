#include "rijden.h"
#include "xbee.h"

Rijden Motors;
Xbee xbee;

unsigned long x = 0;          
const int maxtijd = 500;


void setup() {
  
  

}

void loop() {
 
  
  // Lees alle binnengekomen tekens en onthoud het laatste
  char commando = 0;
  while (xbee.beschikbaar() > 0) {
    commando = xbee.leesXbee();
  }

  if (commando > 0) {
    x = millis();
  }

  switch(commando){
    case 'e': Motors.Stop();
    break;
    
    case 'w': Motors.setSnelheid(300); 
    break;

    case 'a': Motors.setSnelheid(200);
    Motors.naarLinks();
    break;

    case 'd': Motors.setSnelheid(200);
    Motors.naarRechts();
    break;

    case 's': Motors.setSnelheid(300);
     Motors.Achteruit();
    break;      

    case 'q': Motors.setSnelheid(400);
    break;
  }
 
 if(millis() - maxtijd > x){
    Motors.Stop();
  }

  }
