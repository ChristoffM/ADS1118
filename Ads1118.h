/*
   Ads1118.h - library for interacting with TI ADC
   Created by Colby Rome, October 19, 2015
*/
#ifndef Ads1118_h
#define Ads1118_h

#include "Arduino.h"
#include <SPI.h>

class Ads1118
{
    public:
        Ads1118(int CS_pin);
        void begin(); //sets up SPI settings
        word update_config(word new_config); // returns the new config read
                                             // from the onboard register
        word adsRead(word port);
        double readTemp();
        byte self_test(); // returns 0 (no error), 1 (error)
        double convToFloat(word read);
        const word CONFIG_TEMPERATURE = 0x059B;
        const word AIN0 = 0x4000;
        const word AIN1 = 0x5000;
        const word AIN2 = 0x6000;
        const word AIN3 = 0x7000;

    private:
        int _cs;          // chip select
        word CURRENT_CONFIG;
//        const word CONFIG_DEFAULT = 0x048B;
        const word CONFIG_DEFAULT = 0x008B;

        const word PIN_BITMASK = 0x7000;
        const word PGA_BITMASK = 0x0E00;
        const word G_6144 = 0x0000;
        const word G_4096 = 0x0200;
        const word G_2048 = 0x0400;
        const word G_1024 = 0x0600;
        const word G_0512 = 0x0800;
        const word G_0256 = 0x0E00;
};

#endif