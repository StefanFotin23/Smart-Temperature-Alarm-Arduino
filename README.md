# Arduino-Temperature-Alarm-

This device offers real-time data about the temperature measured by an analog temperature sensor and displays that information on an LCD display.
It also has an alarm system that notifies the user through visual (LED) and audio (buzzer) signals when the temperature exceeds a specified limit.
The limit is set by default (in the code) at 24°C and can be modified by the user by pressing the 'A' button on the keypad and then entering the desired temperature limit.
The new limit can have values between 0°C and 99°C. After entering the two digits that form the new temperature ('xx'), the temperature will be automatically set to the new value.


Components: 
  - 2 x Arduino UNO R3
  - LCD 1602 I2C
  - 4×4 Keypad
  - Temperature Sensor
  - Piezo Buzzer
  - NPN Transistor
  - LED
  - Resistors
  - Wires
