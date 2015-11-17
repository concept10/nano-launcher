#ifndef Gauge_h
#define Gauge_h

// Base class for gauges
// J.Ward 17/11/15
class Gauge {
public:
  Gauge() :m_value(0) {}
  
  Gauge & setValue(unsigned value) {
    if (value != m_value) update( m_value, value );
    return *this;
  }
  
  unsigned getValue() const {
    return m_value;
  }
  
protected:
  virtual void update( unsigned oldValue, unsigned newValue ) {
    m_value = newValue;
  }
  
private:
  unsigned m_value;
};

#endif//Gauge_h
