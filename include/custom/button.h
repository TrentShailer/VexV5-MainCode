#ifndef button_h
#define button_h

#include "vex.h"

class Button
{
  public:
    int x, y, width, height;
    std::string text;
    vex::color buttonColor, textColor;
    
    Button(int x, int y, int width, int height, std::string text, vex::color buttonColor, vex::color textColor)
    : x(x), y(y), width(width), height(height), text(text), buttonColor(buttonColor), textColor(textColor){}

    void render()
    {
      Brain.Screen.drawRectangle(x, y, width, height, buttonColor);
      Brain.Screen.printAt(x + 10, y + 10, false, text.c_str());
    }

    bool isClicked()
    {
      if(Brain.Screen.pressing() && Brain.Screen.xPosition() >= x && Brain.Screen.xPosition() <= x + width &&
      Brain.Screen.yPosition() >= y && Brain.Screen.yPosition() <= y + width) return true;
      return false;
    }
};
#endif