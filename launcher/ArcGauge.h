#ifndef ArcGauge_h
#define ArcGauge_h

#include "Gauge.h"
#include "LCD.h"

// Simple class for drawing circular arc gauges with a
// minimalist appearance
// J.Ward 17/11/15

// Arc shaped analogue gauge
class ArcGauge :public Gauge {
  typedef Gauge base;
public:
  ArcGauge(LCD &lcd) :m_lcd(lcd) {
    setExtent(0.f,360.f);
    setPosition(50,50);
    setRadius(40,50);
    setColour(1,2);
    setSteps(36);
  }
  
  // Minimum and maximum angular extent of the gauge
  ArcGauge & setExtent(float a, float b) {
    m_extent[0] = a;
    m_extent[1] = b;
    return *this;
  }
  
  // Coordinates of gauge centre
  ArcGauge & setPosition(int x, int y) {
    m_position[0] = x;
    m_position[1] = y;
    return *this;
  }
  
  // Internal and external radius of the arc
  ArcGauge & setRadius(int a, int b) {
    m_radius[0] = a;
    m_radius[1] = b;
    return *this;
  }
  
  // The colours of the bar and background, respectively
  ArcGauge & setColour(int a, int b) {
    m_colour[0] = a;
    m_colour[1] = b;
    return *this;
  }
  
  // The number of discrete segments used to render the gauge
  ArcGauge & setSteps(int steps) {
    m_steps = steps;
    return *this;
  }
  
  // Draw the gauge. Supplying the previous value allows for a
  // more efficeint partial redraw.
  void draw( int oldValue = -1 ) {
    // calculate the segment where the colour will change
    int edge = m_steps * getValue() / 1000.0 + 0.5;
    
    // select the first colour
    m_lcd.foreground(m_colour[0]);
    
    // angular step size of one segment in degrees
    float stepSize = (m_extent[1] - m_extent[0]) / m_steps;
    
    // range of segments to be updated
    int startValue = 0;
    int endValue = m_steps;
    
    // if an old value has been supplied, we will just do
    // a partial update of the gauge
    if (oldValue >= 0) {
      int oldEdge = m_steps * oldValue / 1000.0 + 0.5;
      startValue = (edge < oldEdge) ? edge : oldEdge;
      endValue   = (edge > oldEdge) ? edge : oldEdge;
    }
    
    // start angle
    float angle = m_extent[0] + startValue * stepSize;
    
    // redraw the arc in steps of stepSize
    for (int i=startValue; i<endValue; i++) {
      if (i==edge) m_lcd.foreground(m_colour[1]);
      m_lcd.arcFill(
        m_position[0], m_position[1], m_radius[0], m_radius[1],
        angle, angle+stepSize
      );
      angle += stepSize;
    }
  }

protected:
  // Called automatically when the gauge value changes
  virtual void update( unsigned oldValue, unsigned newValue ) {
    base::update(oldValue, newValue);
    // partial update of the gauge
    draw(oldValue);
  }
  
private:
  float m_extent[2];
  int   m_position[2];
  int   m_radius[2];
  int   m_colour[2];
  int   m_steps;
  float m_stepSize;
  LCD  &m_lcd;
};

#endif//ArcGauge_h
