/** MASTER **/
#include <Wire.h>

// The slave address to which this master will send data
#define I2C_SLAVE_ADDRESS 0x09

float REF_HIGH_TEMP = 24;
float temp;
int out;
float vout; 
float vout1; 
int LED = 13; 
int piezo = 7;
int o1, o2, o3, o4;

void setup() 
{  
	pinMode(A1, INPUT); 
	pinMode(LED,OUTPUT); 
	pinMode(piezo,OUTPUT);	
	Wire.begin();
} 
void loop() 
{ 
	vout=analogRead(A1); 
	vout1=(vout/1023)*5000; 
	temp=(vout1-500)/10; 
	if (temp>=REF_HIGH_TEMP) {
		digitalWrite(LED,HIGH);
		digitalWrite(piezo,HIGH);
	} else { 
		digitalWrite(LED,LOW);
		digitalWrite(piezo,LOW); 
    }
  
  	out = temp * 100;
  	o1 = out / 1000;
  	o2 = (out / 100) % 10;
  	o3 = (out / 10) % 10;
  	o4 = out % 10;
  
  	Wire.beginTransmission(I2C_SLAVE_ADDRESS);
  	Wire.write(o1);
 	Wire.write(o2);
  	Wire.write(o3);
 	Wire.write(o4);
    Wire.endTransmission();
	delay(5000);
}