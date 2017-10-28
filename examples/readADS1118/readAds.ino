//Example of the ADS118 library by CMuller
//Please change the CS pin to suite your hardware layout
//Tested on ESP8266 (NodeMCU), but should work on all arduino boards

#include "ads1118.h"
#include <SPI.h>
#define CS 15

#define DEBUG

ads1118 ads1118(CS);

void setup() {
  // put your setup code here, to run once:
  ads1118.begin();
  Serial.begin(115200);
  Serial.println("Starting ADS118 example");
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print("ADC0:");
    Serial.println(ads1118.adsRead(ads1118.AIN0), DEC);
    Serial.print("ADC1:");
    Serial.println(ads1118.adsRead(ads1118.AIN1), DEC);
    Serial.print("temperature:");
    Serial.println(ads1118.readTemp(), DEC);
    delay(1000);
}
