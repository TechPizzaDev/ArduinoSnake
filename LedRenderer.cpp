#include "LedRenderer.h"

void LedRenderer::setPixel(int x, int y, bool state)
{
    int dx = x / DISPLAY_WIDTH;
    int dy = y / DISPLAY_HEIGHT;
    int address = LedRenderer::getDisplayAddress(dx, dy);
    byte *rowSlice = getRowBufferSlice(address);

    int column = x % DISPLAY_WIDTH;
    if (state)
        rowSlice[column] |= B10000000 >> column;
    else
        rowSlice[column] &= ~(B10000000 >> column);
}

void LedRenderer::drawBuffer()
{
    for (int dy = 0; dy < DISPLAY_ROWS; dy++)
    {
        for (int dx = 0; dx < DISPLAY_COLUMNS; dx++)
        {
            int address = LedRenderer::getDisplayAddress(dx, dy);
            byte *rowSlice = getRowBufferSlice(address);

            for (int row = 0; row < DISPLAY_HEIGHT; row++)
            {
                byte rowValue = rowSlice[row];
                //rowValue = ((b * 0x0802LU & 0x22110LU) | (b * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
                controller->setRow(address, row, rowValue);
            }
        }
    }
}

byte *LedRenderer::getRowBufferSlice(int address)
{
    return pixelRowBuffer + address * DISPLAY_COLUMNS * DISPLAY_ROWS;
}

LedControl* LedRenderer::getController()
{
    return controller;
}

int LedRenderer::getDisplayAddress(int displayX, int displayY)
{
    return displayY * DISPLAY_COLUMNS + displayX;
}
