#pragma once
#include "LedControl.h"

#define DISPLAY_COLUMNS 2
#define DISPLAY_ROWS 2
#define DISPLAY_WIDTH 8
#define DISPLAY_HEIGHT 8

class LedRenderer
{
private:
  LedControl *controller;
  byte pixelRowBuffer[DISPLAY_COLUMNS * DISPLAY_ROWS * DISPLAY_HEIGHT];

public:
  void setPixel(int x, int y, bool state);
  void drawBuffer();
  byte *getRowBufferSlice(int address);
  LedControl *getController();

  static int getDisplayAddress(int displayX, int displayY);

  LedRenderer(LedControl *controller)
  {
    this->controller = controller;
  }
};
