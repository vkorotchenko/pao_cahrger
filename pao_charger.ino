#include "config.h"

int error_state = 0;

#include <SimpleTimer.h>
#include "mcp2515_can.h"
#include "Logger.h"

#include "config.h"

unsigned char voltamp[8] = {highByte(MAX_VOLTAGE), lowByte(MAX_VOLTAGE), highByte(MAX_AMPS), lowByte(MAX_AMPS), 0x00, 0x00, 0x00, 0x00};

unsigned char len = 0; // Length of received CAN message of either charger
unsigned char buf[8];  // Buffer for data from CAN message of either charger
float pv_voltage;
float pv_current;

    mcp2515_can CAN(SPI_CS_PIN);
SimpleTimer timer;

void canRead()
{

  if (CAN_MSGAVAIL == CAN.checkReceive())
  {
    CAN.readMsgBuf(&len, buf);
    int receiveId = CAN.getCanId();

    if (receiveId == tcc_outgoing_can_id)
    { // CAN Bus ID from TC charger protocol 1430
      for (int i = 0; i < len; i++)

        Logger::log("TC CAN Data received! CAN Id: %X", receiveId);

      Serial.print(" / CAN Data: ");
      for (int i = 0; i < len; i++)
      { // Output data

        if (buf[i] < 0x10)
        { // Displaying zero if only one digit
          Serial.print("0");
        }

        Serial.print(buf[i], HEX);
        Serial.print(" "); // Spaces
      }


      pv_current = (((float)buf[2] * 256.0) + ((float)buf[3])) / 10.0; // highByte/lowByte + offset
      pv_voltage = (((float)buf[0] * 256.0) + ((float)buf[1])) / 10.0; // highByte/lowByte + offset

      Logger::log("Charging voltage: %f V , current: %f A ", pv_voltage, pv_current);
      switch (buf[4])
      { // Read out error byte

      case B00000001:
        error_state = buf[4];
        Logger::log("Error: hardware error");
        break;
      case B00000010:
        error_state = buf[4];
        Logger::log("Error: overheating");
        break;
      case B00000100:
        error_state = buf[4];
        Logger::log("Error: input voltage not allowed");
        break;
      case B00001000:
        error_state = buf[4];
        Logger::log("Error: battery not connected");
        break;
      case B00010000:
        error_state = buf[4];
        Logger::log("Error: CAN bus error");
        break;
      case B00001100:
        error_state = buf[4];
        Logger::log("Error: No input voltage");
        break;
      default:
        error_state = 0;
        break;
      }
    }
  }
}

String canWrite(unsigned char data[8], unsigned long int id)
{

  byte sndStat = CAN.MCP_CAN::sendMsgBuf(id, 1, 8, data); // Send message (ID, extended frame, data length, data)

  if (sndStat == CAN_OK)
  { // Status byte for transmission
    return "CAN message sent successfully to charger";
    error_state = 0;
  }
  else
  {
    return "Error during message transmission to charger ";
    error_state = B00010000;
  }
}

void ledHandler()
{
  switch (error_state)
  { // Read out error byte

  case B00000001:
    blinkIndicatorLeds(1);
    break;
  case B00000010:
    blinkIndicatorLeds(2);
    break;
  case B00000100:
    blinkIndicatorLeds(3);
    break;
  case B00001000:
    blinkIndicatorLeds(4);
    break;
  case B00010000:
    blinkIndicatorLeds(5);
    break;
  case B00001100:
    blinkIndicatorLeds(6);
    break;
  case B00100000:
    blinkIndicatorLeds(7);
    break;
  default:
    setIndicatorLeds();
    break;
  }
}

void blinkIndicatorLeds(int count)
{

  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(ORANGE_PIN, LOW);
  digitalWrite(RED_PIN, HIGH);

  for (int i = 0; i < count; i++)
  {
    digitalWrite(ORANGE_PIN, HIGH);
    delay(200);
    digitalWrite(ORANGE_PIN, LOW);
    delay(200);
  }
}

int getSOC()
{
  if ( pv_voltage < NOMINAL_VOLTAGE * MID_CHARGE_MULTIPLIER) {
    return 1;
  }
  if ( pv_voltage < NOMINAL_VOLTAGE * FULL_CHARGE_MULTIPLIER) {
    return 2;
  }
  if ( pv_voltage > NOMINAL_VOLTAGE * FULL_CHARGE_MULTIPLIER) {
    return 3;
  }
}

void setIndicatorLeds()
{
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(ORANGE_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  int soc = getSOC();

  if (soc < 1)
  {
    digitalWrite(RED_PIN, HIGH);
  }
  if (soc < 2)
  {
    digitalWrite(ORANGE_PIN, HIGH);
  }
  if (soc < 3)
  {
    digitalWrite(GREEN_PIN, HIGH);
  }
}

void tccHandler()
{ // Cyclic function called by the timer

  Logger::log("Set individual charging current: %f A . Target Charging Voltage: %f V ",(float)MAX_AMPS / 10.0, (float)MAX_VOLTAGE / 10.0 ); 

  // Send message and output results
  unsigned char voltamp[8] = {highByte(MAX_VOLTAGE), lowByte(MAX_VOLTAGE), highByte(MAX_AMPS), lowByte(MAX_AMPS), 0x00, 0x00, 0x00, 0x00}; // Regenerate the message
  String canWriteVal = canWrite(voltamp, tcc_incoming_can_id);                                                                                  // Send message and output results
  canRead();                                                                                                                               // Call read function of charger

  Logger::log("canWrite result: %s", canWriteVal); // Print a blank line
}

void setup()
{
  Serial.begin(SERIAL_SPEED);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(ORANGE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(ORANGE_PIN, LOW);
  digitalWrite(RED_PIN, LOW);

  delay(2000);

  while (CAN_OK != CAN.begin(CAN_500KBPS))
  {
    Logger::log("waiting for CAN to intialize");
    digitalWrite(ORANGE_PIN, !digitalRead(ORANGE_PIN));
    delay(200);
  }
  digitalWrite(ORANGE_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);

  Logger::log("CAN initialization successful");
  timer.setInterval(tcc_send_interval, tccHandler);
  timer.setInterval(led_reset_interval, ledHandler);
}

void loop() // ----------------------------LOOP------------------------------//
{
  timer.run();
}
