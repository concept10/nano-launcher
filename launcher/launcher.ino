#include <SoftwareSerial.h>
#include "LCD.h"
#include "ArcGauge.h"

// Compressed Air Rocket Launcher
// Control software for Arduino Nano
// J.Ward 17/11/15

// define the GPIO pins
static const int
  PIN_NRST     = 12, // LCD /RST pin number
  PIN_TX       = 11, // LCD serial TX pin number
  PIN_RX       = 10, // LCD serial RX pin number (unused)
  PIN_OUT_FIRE = 9,  // Fire output (active low)
  PIN_OUT_PUMP = 8,  // Pump output (active low)
  PIN_SW_FIRE  = 2,  // Fire button (momentary, active low)
  PIN_SW_ARM   = 3,  // Arm (toggle switch, active low)
  PIN_SW_PUMP  = 4,  // Pump (toggle switch, active low)
  PIN_SW_KEY   = 5,  // Key switch (latching, active low)
  PIN_PRESSURE = 7;  // Pressure sensor analogue input

// LCD display
// This is a Hobbytronics TFT serial display, with serial
// connection at 9600 baud, and modified firmware for polygon
// drawing. There's no flow control (therefore, a few ugly
// delays are used below)
LCD lcd(PIN_RX, PIN_TX, PIN_NRST);

// analogue gauge
ArcGauge gauge(lcd);

// Arduino setup function
void setup()  
{
    // initialise LCD display
    lcd.init();
    
    // initialise switch inputs
    // all switches are active low (tied to ground), using the
    // internal pull ups on the arduino
    pinMode(PIN_SW_PUMP, INPUT_PULLUP);
    pinMode(PIN_SW_KEY,  INPUT_PULLUP);
    pinMode(PIN_SW_ARM,  INPUT_PULLUP);
    pinMode(PIN_SW_FIRE, INPUT_PULLUP);
    
    // initialise outputs
    // these are active low, and used to control a small
    // 2-channel relay module:
    // * one relay switches a 12V compressor pump
    // * one relay controls a 12V solenoid valve
    // there are 1N4007 diodes across both these loads
    digitalWrite(PIN_OUT_FIRE, HIGH);
    pinMode(PIN_OUT_FIRE, OUTPUT);
    digitalWrite(PIN_OUT_PUMP, HIGH);
    pinMode(PIN_OUT_PUMP, OUTPUT);
    
    // pressure sensor analogue input
    // using a car oil pressure sensor rated as follows:
    // * pressure  : 0-100 psi
    // * resistance: 18-126 ohm
    // wired as a potential divider with 1K resistor to 5V rail
    pinMode(PIN_PRESSURE, INPUT);
 
    // enable portrait mode
    lcd.rotation(0);

    // setup the gauge
    gauge
      .setExtent(30+90,330+90)
      .setPosition(64,80)
      .setRadius(40,60)
      .setColour(2,1)
      .setSteps(30)
      .setValue(0);
    gauge.draw();
    
    delay(50);
}

// convert raw ADC reading from pressure sensor to PSI
// (parameters come from a straight line fit in Excel)
float adc2psi(float adc) {
  return (adc - 5) / 1.31;
}

// Arduino loop function
void loop()
{
  // maximum PSI display on gauge
  int maxPSI = 100;
  
  // read the pressure sensor analogue input
  static int adc = 0;
  adc = (analogRead(PIN_PRESSURE) + adc) / 2;
  
  // convert raw ADC reading to PSI
  int psi = adc2psi(adc) + 0.5f;
  // clamp the PSI readings to maximum range
  if (psi < 0) psi = 0;
  if (psi > maxPSI) psi = maxPSI;
  
  //static float time=0.0;
  //time++;
  //int value = 500.0 + 400.0 * sin(time/10.0);
  
  // convert PSI to value in range 0-1000 for gauge
  int value = (1000.0f * psi / maxPSI) + 0.5f;
  gauge.setValue(value);

  delay(20);
  
  // the various control signals derived from switch state
  bool armed = (digitalRead(PIN_SW_ARM)==LOW);
  bool fire  =  armed && (digitalRead(PIN_SW_FIRE)==LOW);
  bool pump  = !armed && (digitalRead(PIN_SW_PUMP)==LOW);
  bool key   = (digitalRead(PIN_SW_KEY)==LOW);
  
  // convert switch state into text
  char state[5] = {0};
  state[0] = (pump )?'P':'-';
  state[1] = (key  )?'K':'-';
  state[2] = (armed)?'A':'-';
  state[3] = (fire )?'F':'-';
  
  // display switch state
  lcd.foreground(7);
  lcd.gotoXYPixel(42,65);
  lcd.print(state);
  delay(20);
  
  // fire/pump control signals depend on switch state above
  digitalWrite(PIN_OUT_FIRE, fire ? LOW : HIGH);
  digitalWrite(PIN_OUT_PUMP, pump ? LOW : HIGH);
  
  // display the pressure reading in PSI
  lcd.foreground(3);
  lcd.gotoXYPixel(40,85);
  char text[10];
  sprintf(text,"%4d",psi);
  lcd.print(text);
  
  delay(50);
}


