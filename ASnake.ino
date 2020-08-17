#include "LedControl.h"
#include "LedRenderer.h"
#include "SerialRenderer.h"
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

LedRenderer ledRenderer = LedRenderer(&ledController);
SerialRenderer serialRenderer = SerialRenderer();

Renderer *renderer;
Game game = Game();

void setup()
{
    Serial.begin(38400);

    for (int i = 0; i < ledController.getDeviceCount(); i++)
    {
        ledController.setIntensity(i, 7);
        ledController.shutdown(i, false);
    }

    renderer = &serialRenderer;

    Serial.write(SerialOutgoingMessageType::DisplayParameters);
    Serial.write(DISPLAY_COLUMNS * DISPLAY_ROWS);
    Serial.write(DISPLAY_WIDTH);
    Serial.write(DISPLAY_HEIGHT);

    Serial.flush();
}

void loop()
{
    unsigned long currentMilliTime = millis();
    long elapsedMilliTime = currentMilliTime - oldMilliTime;
    accumulatedTime += elapsedMilliTime / 1000.0f;
    oldMilliTime = currentMilliTime;

    if (accumulatedTime >= targetTime)
    {
        game.tick(targetTime);
        game.draw(renderer);

        renderer->commit();

        accumulatedTime -= targetTime;
    }
}
