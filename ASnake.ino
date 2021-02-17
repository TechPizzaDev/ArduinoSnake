#include "LedControl.h"
#include "LedRenderer.h"
#include "SerialRenderer.h"
#include "Game.h"

#define LED_PIN_DATA 4
#define LED_PIN_CLK 3
#define LED_PIN_LOAD 2
#define LED_CONTROLLER_COUNT 4

const int tickRate = 25;
const float targetTime = (1.0 / tickRate);

bool setupAcknowledged;
unsigned long oldMilliTime;
float accumulatedTime;

Game game = Game();

Renderer *renderer1;
Renderer *renderer2;

void setup()
{
    setupAcknowledged = true;
    //if (setupAcknowledged)
    {
        auto ledController = new LedControl(
            LED_PIN_DATA, LED_PIN_CLK, LED_PIN_LOAD, LED_CONTROLLER_COUNT);

        for (int i = 0; i < ledController->getDeviceCount(); i++)
        {
            ledController->setIntensity(i, 7);
            ledController->shutdown(i, false);
        }

        renderer1 = new LedRenderer(ledController);
    }
    //else
    {
        renderer2 = new SerialRenderer();
    }

    Serial.begin(38400);

    Serial.write(SerialOutgoingMessageType::Setup);
    Serial.println("Arduino");
    Serial.write(HORIZONTAL_DISPLAYS);
    Serial.write(VERTICAL_DISPLAYS);
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
        readSerialMessages();

        if (setupAcknowledged)
        {
            game.tick(targetTime);

            game.draw(renderer1);
            renderer1->commit();

            memcpy(
                renderer2->pixelRowBuffer, renderer1->pixelRowBuffer,
                sizeof(renderer2->pixelRowBuffer));

            renderer2->commit();
        }

        accumulatedTime -= targetTime;
    }
}

void readSerialMessages()
{
TryRead:
    int incomingType = Serial.read();
    switch (incomingType)
    {
    case SerialIncomingMessageType::SetupAcknowledge:
        setupAcknowledged = true;
        break;

    case SerialIncomingMessageType::MoveDirection:
        byte direction[1];
        if (Serial.readBytes(direction, 1) == 1){
            game.setDirection(direction[0]);
        }
        break;

    default:
        return;
    }
    goto TryRead;
}
