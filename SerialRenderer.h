#pragma once
#include "Renderer.h"
#include "Structure.h"

class SerialRenderer : public Renderer
{
public:
    SerialRenderer()
    {
    }

    void commit()
    {
        Serial.write(SerialOutgoingMessageType::DisplayData);
        Serial.write(pixelRowBuffer, sizeof(pixelRowBuffer));
    }
};
