/*
   Ads111.cpp - library for interacting with TI ADC
   Created by Colby Rome, October 20, 2015
   Edited by Ward Prescott, March 1, 2016
   Edited by C Muller, Oct 26, 2017
*/

#include "Arduino.h"
#include "ads1118.h"

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x)  Serial.print(x)
#define DEBUG_PRINTLN(x,y)  Serial.println(x,y)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x,y)
#endif

ads1118::ads1118(int CS_pin)    // Constructor for Ads1118 class.
{
  _cs = CS_pin;
}

void ads1118::begin()
{
  // Initializes SPI parameters.
  //Clock Polarity 0, Clock Phase 1 -> SPI mode 1. MSB first.
  //   Page 25 on datasheet
  pinMode(_cs, OUTPUT);
  SPI.begin();
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE1));
  digitalWrite(_cs, HIGH);  // Do not begin transactions yet
  if (self_test()) { //Error
    DEBUG_PRINT("An error has occurred during self test");
  }
}

byte ads1118::self_test()
{
  // This self test returns 1 if there is an error communicating with
  // the ADS1118 chip. This may be due to a wiring problem or other.
  if (CONFIG_RESET != update_config(CONFIG_RESET)) {
    return 1;
  }
  return 0;
}

word ads1118::update_config(uint16_t new_config)
{
  // This function executes a 32-bit transaction.
  //  Input: a word new_config (see datasheet)
  //  Output: a word specifying the configuration register. Should be identical to new_config if config updated correctly.
  word readConfig; //this is the returned config from ADS1118
  digitalWrite(_cs, LOW);//start SPI comms
  delayMicroseconds(1);
  SPI.transfer16(new_config);//first reply is the Data register
  readConfig = SPI.transfer16(new_config);//This reply is the new configuration register
  digitalWrite(_cs, HIGH); //end SPI comms
  return readConfig;
}

word ads1118::adsReadRaw(word port)
{
  word read;
  DEBUG_PRINT("Config RAW= ");
  DEBUG_PRINTLN(CONFIG_SINGLE_START, BIN);
  DEBUG_PRINT("Updating configuration to: ");
  DEBUG_PRINTLN(port | (CONFIG_SINGLE_START & ~(PIN_BITMASK)), BIN);
  digitalWrite(_cs, LOW);//start SPI comms
  delayMicroseconds(1);
  SPI.transfer16(port | (CONFIG_SINGLE_START & ~(PIN_BITMASK)));//start conversion
  digitalWrite(_cs, HIGH);//end SPI comms so that conversion can be completed and data read on the next SPI comms
  delay(10);//wait for a conversion (at 128 SPS)
  digitalWrite(_cs, LOW);
  delayMicroseconds(1);
  read = SPI.transfer16(CONFIG_RESET);//read conversion started previously and leave device in low power mode
  digitalWrite(_cs, HIGH);
  return read;
}

double ads1118::convToFloat(word read)
{
  /* This method returns a double (float) signifying the input signal
     Vin for the proper FS (scaling).

     Input: a binary word from the sensor
     Output: the floating point representation of the sensor value.
  */

  // See page 26 of datasheet detailing the 6 gain modes.
  float gain[6] = {6.144, 4.096, 2.048, 1.024, 0.512, 0.256};
  // Find the correct gain index using bitshift and bitmask
  float myGain = gain[(((PGA_BITMASK & CONFIG_SINGLE_START) >> 8) / 2)];
  DEBUG_PRINT("Gain = ");
  DEBUG_PRINTLN(myGain, DEC);
  return (float)((int)read) * ((float)myGain) / ((float)32768);
}


bool ads1118::setGain(uint16_t GainSet) {
  //Temp variable
  uint16_t newConfig = CONFIG_SINGLE_START;
  //Clear the corresponding bits in current config
  newConfig &= ~(0b111 << 9);
  //Set the corresponding pins in current config
  newConfig |= (GainSet << 9);
  //Send update to the ADC
  if (update_config(newConfig) == newConfig) {
    CONFIG_SINGLE_START = newConfig;
    return true;
  }
  else {
    return false;
  }
}

double ads1118::adsRead(word port)
{
  /* This method returns the floating point representation of the desired
     sensor operation.

     Input: a port selection (see header file)
     Output: the floating point representation of desired operation.
  */
  return convToFloat(adsReadRaw(port)); // Reads from port; converts to float
}

double ads1118::readTemp()
{
  word read;
  digitalWrite(_cs, LOW);
  delayMicroseconds(1);
  SPI.transfer16(CONFIG_TEMPERATURE);//first write to write to register
  digitalWrite(_cs, HIGH);
  delay(10);//wait for temperature conversion
  digitalWrite(_cs, LOW);
  delayMicroseconds(1);
  read = SPI.transfer16(CONFIG_RESET);//then read register and switch device off
  digitalWrite(_cs, HIGH);
  if ((0x8000 & read) == 0)
    return (double)read * 0.0078125; //0.03125;
  else {
    read = ~(read - 1);
    return (double)read * -0.0078125; //-0.03125;
  }
}

