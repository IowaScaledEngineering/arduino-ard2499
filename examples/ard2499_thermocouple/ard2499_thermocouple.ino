/*************************************************************************
Title:    ARD-LTC2499 Thermocouple Example Sketch
Authors:  Nathan D. Holmes <maverick@drgw.net>
File:     $Id: $
License:  GNU General Public License v3

ABOUT:
  This sketch will demonstrate how to use the Ard2499 library for the 
  Iowa Scaled Engineering ARD-LTC2499 shield.   For more information about the
  ARD-LTC2499, see here:  http://www.iascaled.com/store/ARD-LTC2499
  
  The jumpers for ADC address and EEPROM address should be left open, or the 
  defines for the ADC and EEPROM addresses should be changed in the setup() function.

  A Type K thermocouple should be connected between inputs 0 (positive) and 1 (negative)
  on the ARD-LTC2499 for this to work correctly.

  This requires the ISE Thermocouple library as well.
  https://github.com/IowaScaledEngineering/arduino-thermocouple

LICENSE:
    Copyright (C) 2021 Nathan D. Holmes & Michael D. Petersen

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

*************************************************************************/

#include <Wire.h>
#include <Ard2499.h>
#include <Thermocouple.h>

Ard2499 ard2499board1;
TypeK typeK;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  Wire.begin();
  ard2499board1.begin(ARD2499_ADC_ADDR_ZZZ, ARD2499_EEP_ADDR_ZZ);
  ard2499board1.ltc2499ChangeConfiguration(LTC2499_CONFIG2_60_50HZ_REJ);

  // Just verify that we're talking to the ARD-LTC2499
  Serial.print("\n\neeprom mac = [");
  Serial.print(ard2499board1.eui48Get());
  Serial.print("]\n");
}

void loop() {
  // Get the ARD-LTC2499 temperature as the cold junction temp  
  float Tcj = ard2499board1.ltc2499ReadTemperature(ARD2499_TEMP_C);

  // Set for differential measurement - ch0+, ch1 negative
  ard2499board1.ltc2499ChangeChannel(LTC2499_CHAN_DIFF_0P_1N);
  // Get potential from thermocouple
  float Vtc = ard2499board1.ltc2499ReadVoltage() * 1000.0;

  Serial.print("\nCold juct temp = ");
  Serial.print(Tcj, 2);
  Serial.print(" deg C\n");

  Serial.print("Thermocouple voltage = ");
  Serial.print(Vtc, 3);
  Serial.print(" mV\n");

  float Ttc = typeK.getTemperature(Vtc, Tcj);

  Serial.print("Thermocouple temp = ");
  Serial.print(Ttc, 2);
  Serial.print(" deg C\n");
 
  delay(1000);
}
