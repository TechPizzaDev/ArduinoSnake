#include "LedRenderer.h"
#include "LedControl.h"
#include "Game.h"

#define LED_PIN_DATA 4
#define LED_PIN_CLK 3
#define LED_PIN_LOAD 2
#define LED_CONTROLLERS 4

const int tickRate = 25;
const float targetTime = (1.0 / tickRate);

unsigned long oldMilliTime;
float accumulatedTime;

LedControl ledController = LedControl(
  LED_PIN_DATA, LED_PIN_CLK, LED_PIN_LOAD, LED_CONTROLLERS);
  
LedRenderer renderer = LedRenderer(&ledController);
Game game = Game(&renderer);

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < ledController.getDeviceCount(); i++)
  {
    ledController.setIntensity(i, 7);
    ledController.shutdown(i, false);
  }
}

void loop()
{
  unsigned long currentMilliTime = millis();
  int elapsedMilliTime = currentMilliTime - oldMilliTime;
  accumulatedTime += elapsedMilliTime / 1000.0;
  oldMilliTime = currentMilliTime;

  if (accumulatedTime >= targetTime)
  {
    game.tick(targetTime);
    renderer.drawBuffer();

    accumulatedTime -= targetTime;
  }
}
