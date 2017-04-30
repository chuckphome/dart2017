/******************************************************************************
I2C_ReadAllData.ino
BME280 Arduino and Teensy example
Marshall Taylor @ SparkFun Electronics
May 20, 2015
https://github.com/sparkfun/SparkFun_BME280_Arduino_Library
This sketch configures the BME280 to read all measurements.  The sketch also
displays the BME280's physical memory and what the driver perceives the
calibration words to be.
Resources:
Uses Wire.h for I2C operation
Uses SPI.h for SPI operation
Development environment specifics:
Arduino IDE 1.6.4
Teensy loader 1.23
This code is released under the [MIT License](http://opensource.org/licenses/MIT).
Please review the LICENSE.md file included with this example. If you have any questions 
or concerns with licensing, please contact techsupport@sparkfun.com.
Distributed as-is; no warranty is given.
******************************************************************************/

#include <stdint.h>
#include "SparkFunBME280.h"
//Library allows either I2C or SPI, so include both.
#include "Wire.h"
#include "SPI.h"

//Global sensor object
BME280 mySensor;


/*
  SD card test

  created  28 Mar 2011
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  Hookup for the SparkFun microSD shield 
  on a SparkFun RedBoard/Arduino Uno R3

  MOSI - 11
  MISO - 12
  CLK - 13 
  CS - 8
*/

// include the SD library:
#include <SPI.h>
#include <SD.h>

// set up variables using the SD utility library functions:
// Sd2Card card;
// SdVolume volume;
// SdFile root;
extern File dataFile;



void setup_atmo()
{
  //***Driver settings********************************//
  //commInterface can be I2C_MODE or SPI_MODE
  //specify chipSelectPin using arduino pin names
  //specify I2C address.  Can be 0x77(default) or 0x76
  
  //For I2C, enable the following and disable the SPI section
  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x77;
  
  //For SPI enable the following and dissable the I2C section
  //mySensor.settings.commInterface = SPI_MODE;
  //mySensor.settings.chipSelectPin = 10;


  //***Operation settings*****************************//
  
  //renMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  mySensor.settings.runMode = 3; //Normal mode
  
  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  mySensor.settings.tStandby = 0;
  
  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  mySensor.settings.filter = 0;
  
  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
    mySensor.settings.pressOverSample = 1;
  
  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.humidOverSample = 1;

 
  Serial.print("Starting BME280... result of .begin(): 0x");
  
  //Calling .begin() causes the settings to be loaded
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  Serial.println(mySensor.begin(), HEX);

  Serial.print("Displaying ID, reset and ctrl regs\n");
  
  Serial.print("ID(0xD0): 0x");
  Serial.println(mySensor.readRegister(BME280_CHIP_ID_REG), HEX);
  Serial.print("Reset register(0xE0): 0x");
  Serial.println(mySensor.readRegister(BME280_RST_REG), HEX);
  Serial.print("ctrl_meas(0xF4): 0x");
  Serial.println(mySensor.readRegister(BME280_CTRL_MEAS_REG), HEX);
  Serial.print("ctrl_hum(0xF2): 0x");
  Serial.println(mySensor.readRegister(BME280_CTRL_HUMIDITY_REG), HEX);

  Serial.print("\n\n");

  Serial.print("Displaying all regs\n");
  uint8_t memCounter = 0x80;
  uint8_t tempReadData;
  for(int rowi = 8; rowi < 16; rowi++ )
  {
    Serial.print("0x");
    Serial.print(rowi, HEX);
    Serial.print("0:");
    for(int coli = 0; coli < 16; coli++ )
    {
      tempReadData = mySensor.readRegister(memCounter);
      Serial.print((tempReadData >> 4) & 0x0F, HEX);//Print first hex nibble
      Serial.print(tempReadData & 0x0F, HEX);//Print second hex nibble
      Serial.print(" ");
      memCounter++;
    }
    Serial.print("\n");
  }
  
  
  Serial.print("\n\n");
  
  Serial.print("Displaying concatenated calibration words\n");
  Serial.print("dig_T1, uint16: ");
  Serial.println(mySensor.calibration.dig_T1);
  Serial.print("dig_T2, int16: ");
  Serial.println(mySensor.calibration.dig_T2);
  Serial.print("dig_T3, int16: ");
  Serial.println(mySensor.calibration.dig_T3);
  
  Serial.print("dig_P1, uint16: ");
  Serial.println(mySensor.calibration.dig_P1);
  Serial.print("dig_P2, int16: ");
  Serial.println(mySensor.calibration.dig_P2);
  Serial.print("dig_P3, int16: ");
  Serial.println(mySensor.calibration.dig_P3);
  Serial.print("dig_P4, int16: ");
  Serial.println(mySensor.calibration.dig_P4);
  Serial.print("dig_P5, int16: ");
  Serial.println(mySensor.calibration.dig_P5);
  Serial.print("dig_P6, int16: ");
  Serial.println(mySensor.calibration.dig_P6);
  Serial.print("dig_P7, int16: ");
  Serial.println(mySensor.calibration.dig_P7);
  Serial.print("dig_P8, int16: ");
  Serial.println(mySensor.calibration.dig_P8);
  Serial.print("dig_P9, int16: ");
  Serial.println(mySensor.calibration.dig_P9);
  
  Serial.print("dig_H1, uint8: ");
  Serial.println(mySensor.calibration.dig_H1);
  Serial.print("dig_H2, int16: ");
  Serial.println(mySensor.calibration.dig_H2);
  Serial.print("dig_H3, uint8: ");
  Serial.println(mySensor.calibration.dig_H3);
  Serial.print("dig_H4, int16: ");
  Serial.println(mySensor.calibration.dig_H4);
  Serial.print("dig_H5, int16: ");
  Serial.println(mySensor.calibration.dig_H5);
  Serial.print("dig_H6, uint8: ");
  Serial.println(mySensor.calibration.dig_H6);
    
  Serial.println();

}


void log_atmo()
{
  float temp, pressure, altitude, humidity;
  unsigned long ms;


     
  ms = millis();
  
  //Each loop, take a reading.
  //Start with temperature, as that data is needed for accurate compensation.
  //Reading the temperature updates the compensators of the other functions
  //in the background.

  temp     = mySensor.readTempC();
  pressure = mySensor.readFloatPressure();
  altitude = mySensor.readFloatAltitudeMeters();
  humidity = mySensor.readFloatHumidity();
  if ((loopCount %SERIALDELAY) == 0)
  {
    // to console, as to file:
    Serial.print("\t#TPAH, ");
    Serial.print(temp, 2);
    Serial.print(", ");
    Serial.print(pressure, 2);
    Serial.print(", ");
    Serial.print(altitude, 2);
    Serial.print(", ");
    Serial.print(humidity, 2);
    Serial.println(",");
  }
  if (dataFile)
  {
    // if the file is available, write to it:
    dataFile.print("\t#TPAH, ");
    dataFile.print(temp, 2);
    dataFile.print(", ");
    dataFile.print(pressure, 2);
    dataFile.print(", ");
    dataFile.print(altitude, 2);
    dataFile.print(", ");
    dataFile.print(humidity, 2);
    dataFile.println(", ");
  }
 
}

