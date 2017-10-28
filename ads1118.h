/*
   Ads1118.h - library for interacting with TI ADC
   Created by Colby Rome, October 19, 2015
   Edited by Ward Prescott, March 1, 2016
   Edited by C Muller, Oct 26, 2017
*/

#ifndef ads1118_h
#define ads1118_h

#include "Arduino.h"
#include <SPI.h>

class ads1118
{
  public:
    ads1118(int CS_pin);
    void begin();
    double adsRead(word port);
    double readTemp();
    bool setGain(uint16_t GainSet);
    byte self_test();
    word update_config(uint16_t new_config);
    word adsReadRaw(word port);
    double convToFloat(word read);
    //These are defined gain settings.
    const uint8_t G6_144 = 0b000;
    const uint8_t G4_096 = 0b001;
    const uint8_t G2_048 = 0b010;
    const uint8_t G1_024 = 0b011;
    const uint8_t G0_512 = 0b100;
    //const uint8_t G0_256 = 0b101;  //Not working!?
    //These are defined input pins, diff and single ended.
    const uint16_t DIF01 = 0x0000;
    const uint16_t DIF23 = 0x3000;
    const word AIN0 = 0x4000;
    const word AIN1 = 0x5000;
    const word AIN2 = 0x6000;
    const word AIN3 = 0x7000;
  private:
    int _cs;                // chip select
    const word CONFIG_RESET = 0x058B;
    word CONFIG_SINGLE_START = 0b1000000110001011;
    const word CONFIG_TEMPERATURE  = 0b1000010110011011;
    // Bit masks
    const word PIN_BITMASK = 0x7000;//=0b0111 0000 0000 0000
    const word PGA_BITMASK = 0x0E00;//=0b0000 1110 0000 0000
};

#endif
