#include <Arduino.h>
#include <EEPROM.h>
#include <duinocom.h>

#include "Common.h"
#include "SoilMoistureSensor.h"
#include "Irrigation.h"

//#define SERIAL_MODE_CSV 1
//#define SERIAL_MODE_QUERYSTRING 2
//int serialMode = SERIAL_MODE_CSV;

#define VERSION "1-0-0-1"
#define BOARD_TYPE "uno"

void setup()
{
  Serial.begin(9600);

  Serial.println("Starting irrigator");
  
  serialPrintDeviceInfo();

  setupSoilMoistureSensor();

  setupIrrigation();

  serialOutputIntervalInSeconds = soilMoistureSensorReadingIntervalInSeconds;

  Serial.println("Online");
}

void loop()
{
// Disabled. Used for debugging
//  Serial.print(".");

  if (isDebugMode)
    loopNumber++;

  serialPrintLoopHeader();

  checkCommand();

  takeSoilMoistureSensorReading();

  serialPrintData();

  irrigateIfNeeded();

  serialPrintLoopFooter();

  delay(1);
}

void serialPrintDeviceInfo()
{
  Serial.println("");
  Serial.println("-- Start Device Info");
  Serial.println("Family: GrowSense");
  Serial.println("Group: irrigator");
  Serial.println("Project: SoilMoistureSensorCalibratedPump");
  Serial.print("Board: ");
  Serial.println(BOARD_TYPE);
  Serial.print("Version: ");
  Serial.println(VERSION);
  Serial.println("ScriptCode: irrigator");
  Serial.println("-- End Device Info");
  Serial.println("");
}

/* Commands */
void checkCommand()
{
//  if (isDebugMode)
//  {
//    Serial.println("Checking incoming serial commands");
//  }

  if (checkMsgReady())
  {
    char* msg = getMsg();
        
    char letter = msg[0];

    int length = strlen(msg);

    Serial.print("Received message: ");
    Serial.println(msg);

    switch (letter)
    {
      case '#':
        serialPrintDeviceInfo();
        break;
      case 'P':
        setPumpStatus(msg);
        break;
      case 'T':
        setThreshold(msg);
        break;
      case 'D':
        setDrySoilMoistureCalibrationValue(msg);
        break;
      case 'W':
        setWetSoilMoistureCalibrationValue(msg);
        break;
      case 'I':
        setSoilMoistureSensorReadingInterval(msg);
        break;
      case 'B':
        setPumpBurstOnTime(msg);
        break;
      case 'O':
        setPumpBurstOffTime(msg);
        break;
      case 'X':
        restoreDefaultSettings();
        break;
      case 'N':
        Serial.println("Turning pump on");
        pumpStatus = PUMP_STATUS_ON;
        pumpOn();
        break;
      case 'F':
        Serial.println("Turning pump off");
        pumpStatus = PUMP_STATUS_OFF;
        pumpOff();
        break;
      case 'A':
        Serial.println("Turning pump to auto");
        pumpStatus = PUMP_STATUS_AUTO;
        irrigateIfNeeded();
        break;
      case 'Z':
        Serial.println("Toggling IsDebug");
        isDebugMode = !isDebugMode;
        break;
      case 'R':
        reverseSoilMoistureCalibrationValues();
        break;
    }
    forceSerialOutput();
  }
  delay(1);
}

/* Settings */
void restoreDefaultSettings()
{
  Serial.println("Restoring default settings");

  restoreDefaultSoilMoistureSensorSettings();
  restoreDefaultIrrigationSettings();
}

/* Serial Output */
void serialPrintData()
{
  bool isTimeToPrintData = millis() - lastSerialOutputTime >= secondsToMilliseconds(serialOutputIntervalInSeconds)
      || lastSerialOutputTime == 0;

  bool isReadyToPrintData = isTimeToPrintData && soilMoistureSensorReadingHasBeenTaken;

  if (isReadyToPrintData)
  {
//    if (serialMode == SERIAL_MODE_CSV)
//    {
      Serial.print("D;"); // This prefix indicates that the line contains data.
      Serial.print("R:");
      Serial.print(soilMoistureLevelRaw);
      Serial.print(";C:");
      Serial.print(soilMoistureLevelCalibrated);
      Serial.print(";T:");
      Serial.print(threshold);
      Serial.print(";P:");
      Serial.print(pumpStatus);
      Serial.print(";I:");
      Serial.print(soilMoistureSensorReadingIntervalInSeconds);
      Serial.print(";B:");
      Serial.print(pumpBurstOnTime);
      Serial.print(";O:");
      Serial.print(pumpBurstOffTime);
      Serial.print(";WN:"); // Water needed
      Serial.print(soilMoistureLevelCalibrated < threshold);
      Serial.print(";PO:"); // Pump on
      Serial.print(pumpIsOn);
      Serial.print(";D:"); // Dry calibration value
      Serial.print(drySoilMoistureCalibrationValue);
      Serial.print(";W:"); // Wet calibration value
      Serial.print(wetSoilMoistureCalibrationValue);
      Serial.print(";V:");
      Serial.print(VERSION);
      Serial.print(";;");
      Serial.println();
//    }
    /*else
    {
      Serial.print("raw=");
      Serial.print(soilMoistureLevelRaw);
      Serial.print("&");
      Serial.print("calibrated=");
      Serial.print(soilMoistureLevelCalibrated);
      Serial.print("&");
      Serial.print("threshold=");
      Serial.print(threshold);
      Serial.print("&");
      Serial.print("waterNeeded=");
      Serial.print(soilMoistureLevelCalibrated < threshold);
      Serial.print("&");
      Serial.print("pumpStatus=");
      Serial.print(pumpStatus);
      Serial.print("&");
      Serial.print("readingInterval=");
      Serial.print(soilMoistureSensorReadingIntervalInSeconds);
      Serial.print("&");
      Serial.print("pumpBurstOnTime=");
      Serial.print(pumpBurstOnTime);
      Serial.print("&");
      Serial.print("pumpBurstOffTime=");
      Serial.print(pumpBurstOffTime);
      Serial.print("&");
      Serial.print("pumpOn=");
      Serial.print(pumpIsOn);
      Serial.print("&");
      Serial.print("secondsSincePumpOn=");
      Serial.print((millis() - lastPumpFinishTime) / 1000);
      Serial.print("&");
      Serial.print("dry=");
      Serial.print(drySoilMoistureCalibrationValue);
      Serial.print("&");
      Serial.print("wet=");
      Serial.print(wetSoilMoistureCalibrationValue);
      Serial.print(";;");
      Serial.println();
    }*/

/*    if (isDebugMode)
    {
      Serial.print("Last pump start time:");
      Serial.println(pumpStartTime);
      Serial.print("Last pump finish time:");
      Serial.println(lastPumpFinishTime);
    }*/

    lastSerialOutputTime = millis();
  }
/*  else
  {
    if (isDebugMode)
    {    
      Serial.println("Not ready to serial print data");

      Serial.print("  Is time to serial print data: ");
      Serial.println(isTimeToPrintData);
      if (!isTimeToPrintData)
      {
        Serial.print("    Time remaining before printing data: ");
        Serial.print(millisecondsToSecondsWithDecimal(lastSerialOutputTime + secondsToMilliseconds(serialOutputIntervalInSeconds) - millis()));
        Serial.println(" seconds");
      }
      Serial.print("  Soil moisture sensor reading has been taken: ");
      Serial.println(soilMoistureSensorReadingHasBeenTaken);
      Serial.print("  Is ready to print data: ");
      Serial.println(isReadyToPrintData);

    }
  }*/
}
