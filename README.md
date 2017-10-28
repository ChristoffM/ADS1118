# ADS1118 Arduino Library

Created By: Colby Rome (cdr013@bucknell.edu)

Later Updated By: Ward Prescott (erp006@bucknell.edu) on Mar 9, 2016 12:08 AM

Last Updated By: C Muller in October 2017

The last major edit changed conversions to single-sample (or low power) mode.
Contunuous mode was removed.
Temperature sensing works, but might have errors.
Gain adjustments were not tested.

## ADS1118 Class
### Constructor
`Ads1118(int CS_pin)`
### Methods
- `.begin(void)`
- `adsRead(int port)`returns the floating point representation of the voltage at the port.  Macros for the different ports are defined in the header file and are:
  - DIF01
  - DIF23
  - AIN0
  - AIN1
  - AIN2
  - AIN3
- `readTemp(void)`
- `setGain(int GainSet)`
- `selfTest(void)` the method returns1 if there is an error communicating with the device.  This may be due to a wiring problem or other SPI error.

### Parameters
There no public parameters of interest.

## Usage
Clone repository into your Arduino libraries folder (typically ~/Arduino/libraries/)

See the examples folder.

## Notes
None yet.

## Further Work
1. Check if temperature readings work correctly below 0
2. Change sampling rates, and implement continuous mode if needed
