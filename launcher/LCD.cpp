#include "LCD.h"

// Simple class to drive the Hobbytronics TFTSerial display,
// adding some additional functions such as filled arc drawing.
// This actually relies on an updated firmware image for the
// LCD to support polygon drawing (otherwise this would be
// horrendously slow over 9600 baud serial)
// J.Ward 17/11/15

void LCD::init()
{
  reset();
  m_serial.begin(9600);
  clear(0);
}

void LCD::reset()
{
  pinMode(m_resetPin, OUTPUT);
  digitalWrite(m_resetPin, HIGH);
  delay(1);
  digitalWrite(m_resetPin, LOW);
  delay(1);
  digitalWrite(m_resetPin, HIGH);
  delay(5000);
}

void LCD::clear(byte colour)
{
  background(colour);
  command(0);
}

void LCD::foreground(byte colour)
{
  LCD::command(1,colour);
}

void LCD::background(byte colour)
{
  LCD::command(2,colour);
}

void LCD::rotation(byte rotation)
{
  LCD::command(3,rotation);
}

void LCD::fontSize(byte fontSize)
{
  LCD::command(4,fontSize);
}

void LCD::drawLine(byte x1, byte y1, byte x2, byte y2)
{
  LCD::command(8,x1,y1,x2,y2);
}

void LCD::drawBox(byte x1, byte y1, byte x2, byte y2)
{
  LCD::command(9,x1,y1,x2,y2);
}

void LCD::drawFilledBox(byte x1, byte y1, byte x2, byte y2)
{
  LCD::command(10,x1,y1,x2,y2);
}

void LCD::drawCircle(byte x, byte y, byte radius)
{
  LCD::command(11,x,y,radius);
}

void LCD::drawFilledCircle(byte x, byte y, byte radius)
{
  LCD::command(12,x,y,radius);
}

void LCD::drawQuad(byte x[4], byte y[4])
{
  m_serial.write(0x1B);
  m_serial.write(22);
  for (int i=0;i<4;i++) {
    m_serial.write(x[i]);
    m_serial.write(y[i]);
  }
  m_serial.write(0xFF);
}

void LCD::fillQuad(byte x[4], byte y[4])
{
  m_serial.write(0x1B);
  m_serial.write(23);
  for (int i=0;i<4;i++) {
    m_serial.write(x[i]);
    m_serial.write(y[i]);
  }
  m_serial.write(0xFF);
}

void LCD::gotoXYText(byte x, byte y)
{
  LCD::command(6,x,y);
}

void LCD::gotoXYPixel(byte x, byte y)
{
  LCD::command(7,x,y);
}

void LCD::print(char *text)
{
  if (text == 0) return;
  for (int n=0; text[n]; n++)
    m_serial.write(text[n]);
}

void LCD::setBacklight(byte percent)
{
  LCD::command(14,percent);
}

void LCD::arcFill(byte x, byte y, byte r1, byte r2, float a1, float a2)
{
  a1 = PI * a1 / 180.0;
  a2 = PI * a2 / 180.0;
  
  float c1 = cos(a1), s1 = sin(a1);
  float c2 = cos(a2), s2 = sin(a2);

  byte xb[4], yb[4];
  
  xb[0] = x+r1*c1+0.5; yb[0] = y+r1*s1+0.5;
  xb[1] = x+r2*c1+0.5, yb[1] = y+r2*s1+0.5;
  xb[2] = x+r2*c2+0.5, yb[2] = y+r2*s2+0.5;
  xb[3] = x+r1*c2+0.5, yb[3] = y+r1*s2+0.5;
  
  fillQuad(xb, yb);
} 

void LCD::command(byte command)
{
  m_serial.write(0x1B);
  m_serial.write(command);
  m_serial.write(0xFF);
}

void LCD::command(byte command, byte p1)
{
  m_serial.write(0x1B);
  m_serial.write(command);
  m_serial.write(p1);
  m_serial.write(0xFF);
}

void LCD::command(byte command, byte p1, byte p2)
{
  m_serial.write(0x1B);
  m_serial.write(command);
  m_serial.write(p1);
  m_serial.write(p2);
  m_serial.write(0xFF);
}

void LCD::command(byte command, byte p1, byte p2, byte p3)
{
  m_serial.write(0x1B);
  m_serial.write(command);
  m_serial.write(p1);
  m_serial.write(p2);
  m_serial.write(p3);
  m_serial.write(0xFF);
}

void LCD::command(byte command, byte p1, byte p2, byte p3, byte p4)
{
  m_serial.write(0x1B);
  m_serial.write(command);
  m_serial.write(p1);
  m_serial.write(p2);
  m_serial.write(p3);
  m_serial.write(p4);
  m_serial.write(0xFF);
}

