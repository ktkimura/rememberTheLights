///////////////////////////////////////////////////////////////////////////////
// Circuit Playground Total Acceleration
//
// Author: Carter Nelson
// MIT License (https://opensource.org/licenses/MIT)

#include <Adafruit_CircuitPlayground.h>

float X, Y, Z, totalAccel;
  
///////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  CircuitPlayground.begin();
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_8_G);
}
///////////////////////////////////////////////////////////////////////////////


void loop() 
{
///////////////////////////////////////////////////////////////////////////////
  X = 0;
  Y = 0;
  Z = 0;
  for (int i=0; i<10; i++) {
    X += CircuitPlayground.motionX();
    Y += CircuitPlayground.motionY();
    Z += CircuitPlayground.motionZ();
    delay(1);
  }
  X /= 10;
  Y /= 10;
  Z /= 10;

  totalAccel = sqrt(X*X + Y*Y + Z*Z);
///////////////////////////////////////////////////////////////////////////////

  int ledValue = map(totalAccel, 9, 50, 10, 255);
  
  if(ledValue < 10)
  {
    ledValue = 10;
  }
  if(ledValue > 255)
  {
    ledValue = 255;
  }

  for(int i = 0; i < 10; i++)
  {
    if(totalAccel > 35)
    {
      CircuitPlayground.setPixelColor(i, ledValue, 0, ledValue);
      delay(100);
    }
    else
    {
      CircuitPlayground.setPixelColor(i, 0, 0, ledValue);
      delay(100);
    }
  }
}