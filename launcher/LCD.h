#ifndef LCD_h
#define LCD_h

#include "Arduino.h"
#include <SoftwareSerial.h>

// Simple class to drive the Hobbytronics TFTSerial display,
// adding some additional functions such as filled arc drawing.
// This actually relies on an updated firmware image for the
// LCD to support polygon drawing (otherwise this would be
// horrendously slow over 9600 baud serial)
// J.Ward 17/11/15
class LCD {
public:
  LCD(int rxPin, int txPin, int resetPin) :
    m_serial(rxPin,txPin),
    m_resetPin(resetPin)
  {}
  
  void init();
  void reset();
  void clear(byte colour);
  void foreground(byte colour);
  void background(byte colour);
  void rotation(byte rotation);
  void fontSize(byte fontSize);
  void drawLine(byte x1, byte y1, byte x2, byte y2);
  void drawBox(byte x1, byte y1, byte x2, byte y2);
  void drawFilledBox(byte x1, byte y1, byte x2, byte y2);
  void drawCircle(byte x, byte y, byte radius);
  void drawFilledCircle(byte x, byte y, byte radius);
  void drawQuad(byte x[4], byte y[4]);
  void fillQuad(byte x[4], byte y[4]);
  
  void gotoXYText(byte x, byte y);
  void gotoXYPixel(byte x, byte y);
  void print(char *text);
  void setBacklight(byte percent);
  
  void arcFill(byte x, byte y, byte r1, byte r2, float a1, float a2);

private:
  void command(byte command);
  void command(byte command, byte p1);
  void command(byte command, byte p1, byte p2);
  void command(byte command, byte p1, byte p2, byte p3);
  void command(byte command, byte p1, byte p2, byte p3, byte p4);
  
private:
  SoftwareSerial m_serial;
  int m_resetPin;
};

#endif//LCD_h
