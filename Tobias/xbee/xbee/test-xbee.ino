#include <Arduino.h>
#include "xbee.h"

XBee xbee(9600);

void setup() {
    Serial.begin(9600);
    xbee.init();
    delay(1000);
    while (xbee.available()) {
        xbee.receive();       // gooi opstartdata weg
    }
    xbee.sendString("=== XBee test ===\r\n");
    
}

void loop() {
    if (xbee.available()) {
        char cmd = xbee.receive();
        Serial.print("Ontvangen: ");
        Serial.println(cmd);
        xbee.sendString("Ontvangen: ");
        xbee.sendChar(cmd);
        xbee.sendString("\r\n");
    }
}