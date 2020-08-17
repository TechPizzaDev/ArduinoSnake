#pragma once
#include "Renderer.h"
#include "LedControl.h"

class LedRenderer : public Renderer
{
private:
    LedControl *controller;

public:
    LedRenderer(LedControl *controller)
    {
        this->controller = controller;
    }

    LedControl *getController()
    {
        return controller;
    }

    void commit()
    {
        for (int dy = 0; dy < VERTICAL_DISPLAYS; dy++)
        {
            for (int dx = 0; dx < HORIZONTAL_DISPLAYS; dx++)
            {
                int displayAddress = Renderer::getDisplayAddress(dx, dy);
                byte *display = getDisplay(displayAddress);

                for (int row = 0; row < DISPLAY_HEIGHT; row++)
                {
                    byte value = display[row];

                    // Reverse bits of value.
                    //value = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;

                    controller->setRow(displayAddress, row, value);
                }
            }
        }
    }
};
