#pragma once

#define DISPLAY_COLUMNS 2
#define DISPLAY_ROWS 2
#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 8

class Renderer
{
protected:
    byte pixelRowBuffer[DISPLAY_COLUMNS * DISPLAY_ROWS * DISPLAY_HEIGHT];

public:
    /* 
     * Sends the pixel buffer data to the output.
     */
    virtual void commit() = 0;

    byte *getDisplayColumn(int displayAddress)
    {
        return pixelRowBuffer + displayAddress * DISPLAY_COLUMNS * DISPLAY_ROWS;
    }

    void setPixel(int x, int y, bool state)
    {
        int dx = x / DISPLAY_WIDTH;
        int dy = y / DISPLAY_HEIGHT;
        int displayAddress = Renderer::getDisplayColumnAddress(dx, dy);
        byte *displayColumn = getDisplayColumn(displayAddress);

        int bufferColumn = x % DISPLAY_WIDTH;
        if (state)
            displayColumn[bufferColumn] |= B10000000 >> bufferColumn;
        else
            displayColumn[bufferColumn] &= ~(B10000000 >> bufferColumn);
    }

    static int getDisplayColumnAddress(int displayX, int displayY)
    {
        return displayX + displayY * DISPLAY_COLUMNS;
    }
};