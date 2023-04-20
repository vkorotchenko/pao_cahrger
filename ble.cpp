#include "ble.h"

Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void Ble::setup() {

  if ( !ble.begin(VERBOSE_MODE) )
  {
    Logger::log("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
  }

  /* Perform a factory reset to make sure everything is in a known state */
  Logger::log("Performing a factory reset: ");
  if (! ble.factoryReset() ){
       Logger::log("Couldn't factory reset");
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Logger::log("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Logger::log("Setting device name to %s': ", DISPLAY_NAME);

  if (! ble.sendCommandCheckOK(F(String("AT+GAPDEVNAME=").concat(DISPLAY_NAME))) ) {
    Logger::log("Could not set device name?");
  }

//   /* Add the Heart Rate Service definition */
//   /* Service ID should be 1 */
//   Logger::log("Adding the Heart Rate Service definition (UUID = 0x180D): ");
//   success = ble.sendCommandWithIntReply( F("AT+GATTADDSERVICE=UUID=0x180D"), &hrmServiceId);
//   if (! success) {
//     Logger::log("Could not add HRM service");
//   }

  /* Reset the device for the new service setting changes to take effect */
  Serial.print(F("Performing a SW reset (service changes require a reset): "));
  ble.reset();
}

void Ble::loop(int tVolt, int tAmp, float cVolt, float cAmp){
    ble.print( F("AT+GATTCHAR=") );
    ble.print( tVolt, DEC);
    ble.print( F(",") );
    ble.print( tAmp, DEC);
    ble.print( F(",") );
    ble.print( cVolt, 1);
    ble.print( F(",") );
    ble.print( cAmp, 1);
    ble.println( F(";") );

}
