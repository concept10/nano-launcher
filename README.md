# nano-launcher
Arduino Nano based Compressed Air Rocket Launcher

Simple controller for a compressed air rocket, operated by an Arduino Nano board.
There are several switches used to control the rocket:
* Key switch to select low/high pressure modes (safety feature)
* Fire button (push button, triggers the solenoid valve)
* Arm switch (toggle switch, enables the fire button)
* Pump switch (toggle switch, enables the compressor pump to pressurise the launcher)
All switches are active low, and pull invidivual GPIO pins to ground. Internal GPIO pull-ups are used, rather than external resistors.

There's a 2-channel relay board used to switch a 12V car tyre compressor (100psi maximum pressure) to pressurise the system, and a 12V solenoid valve to launch. These are controlled by two active low GPIO pins. Power is supplied by an old ATX power supply, salvaged from a PC.

Pressure is measured using a car oil pressure gauge (rated 0-100psi and 18-126ohm). This is connected to an analogue input, with a 1K resistor to the 5V rail forming a potential divider. ADC raw values were recorded at several 10psi increments, and a straight line fitted in Excel to get the (hard coded) parameters needed to approximately convert ADC values to PSI.

There's an LCD display (Hobbytronics TFT Serial) used to display a pressure gauge and status. This is connected to a GPIO pin, and controlled using SoftwareSerial at 9600 baud. The LCD has modified firmware to include a simple quad (polygon) filling routine (used to draw the pressure gauge). The firmware will be committed separately on Github at some stage.

The GPIO pin assignments are commented in the source code.
