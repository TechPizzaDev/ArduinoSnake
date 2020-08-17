#pragma once
#include "Renderer.h"
#include "Game.h"

class SerialRenderer : public Renderer
{
public:
    SerialRenderer()
    {
    }

    virtual void commit()
    {
        Serial.write(SerialOutgoingMessageType::DisplayData);

        for (int dy = 0; dy < DISPLAY_ROWS; dy++)
        {
            for (int dx = 0; dx < DISPLAY_COLUMNS; dx++)
            {
                int displayAddress = Renderer::getDisplayColumnAddress(dx, dy);
                byte *displayColumn = getDisplayColumn(displayAddress);

                Serial.write(displayColumn, DISPLAY_HEIGHT);
            }
        }
    }
};
