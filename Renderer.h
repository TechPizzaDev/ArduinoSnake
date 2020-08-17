#pragma once

#define HORIZONTAL_DISPLAYS 2
#define VERTICAL_DISPLAYS 2
#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 8

class Renderer
{
public:
    byte pixelRowBuffer[HORIZONTAL_DISPLAYS * VERTICAL_DISPLAYS * DISPLAY_HEIGHT];

    Renderer()
    {
        memset(pixelRowBuffer, 0, HORIZONTAL_DISPLAYS * VERTICAL_DISPLAYS * DISPLAY_HEIGHT);
    }

    /* 
     * Sends the pixel buffer data to the output.
     */
    virtual void commit() = 0;

    byte *getDisplay(int displayAddress)
    {
        return pixelRowBuffer + displayAddress * DISPLAY_HEIGHT;
    }

    void setPixel(int x, int y, bool state)
    {
        int dx = x / DISPLAY_WIDTH;
        int dy = y / DISPLAY_HEIGHT;
        int displayAddress = Renderer::getDisplayAddress(dx, dy);
        byte *display = getDisplay(displayAddress);

        int bufferColumn = x % DISPLAY_WIDTH;
        int bufferRow = y % DISPLAY_HEIGHT;
        if (state)
            display[bufferRow] |= B10000000 >> bufferColumn;
        else
            display[bufferRow] &= ~(B10000000 >> bufferColumn);
    }

    static int getDisplayAddress(int displayX, int displayY)
    {
        return displayX + displayY * HORIZONTAL_DISPLAYS;
    }
};