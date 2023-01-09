/** SLAVE **/
#include <LiquidCrystal.h>
#include <Wire.h>

// The slave address to which this master will send data
#define I2C_SLAVE_ADDRESS 0x09

// The byte variable in which I2C reads from the master are performed
int i;
signed int aux;
int temp;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  	lcd.begin(16, 2);
  	lcd.print("Buna ziua!");
	Wire.begin(I2C_SLAVE_ADDRESS);
}

void receiveEvent(int numBytes) {
  	i = 3;
  	temp = 0;
  	while (0 <= i) {
      	aux = Wire.read();
      	if (aux > 127) {
      		aux = aux - 256;
      	}
  		temp = temp + pow(10, i) * aux;
      	i--;
    }
}

void loop() {
  	Wire.onReceive(receiveEvent);
  	if (temp != 0) {
      	lcd.clear();
  		lcd.print("Temp refresh");
  		delay(1000);
      	lcd.clear();
  		lcd.print(temp / 100);
  		lcd.print(".");
		lcd.print(abs(temp % 100));
  		lcd.print(" grade C");
      	delay(5000);
    }
}