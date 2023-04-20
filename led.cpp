#include "led.h"

Led::Led(int green, int amber, int red) {
    greenPin = green;
    amberPin = amber;
    redPin = red;
}

void Led::setup()
{
  pinMode(greenPin, OUTPUT);
  pinMode(amberPin, OUTPUT);
  pinMode(redPin, OUTPUT);

  digitalWrite(greenPin, LOW);
  digitalWrite(amberPin, LOW);
  digitalWrite(redPin, LOW);
}

void Led::loop(int errorState, int soc){
    ledHandler(errorState, soc);
}



void Led::ledHandler(int errorState, int soc)
{
  switch (errorState)
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
    setIndicatorLeds(soc);
    break;
  }
}

void Led::setIndicatorLeds(int soc)
{
  digitalWrite(greenPin, LOW);
  digitalWrite(amberPin, LOW);
  digitalWrite(redPin, LOW);

  if (soc < 1)
  {
    digitalWrite(redPin, HIGH);
  }
  if (soc < 2)
  {
    digitalWrite(amberPin, HIGH);
  }
  if (soc < 3)
  {
    digitalWrite(greenPin, HIGH);
  }
}

void Led::blinkIndicatorLeds(int count)
{

  digitalWrite(greenPin, LOW);
  digitalWrite(amberPin, LOW);
  digitalWrite(redPin, HIGH);

  for (int i = 0; i < count; i++)
  {
    digitalWrite(amberPin, HIGH);
    delay(200);
    digitalWrite(amberPin, LOW);
    delay(200);
  }
}