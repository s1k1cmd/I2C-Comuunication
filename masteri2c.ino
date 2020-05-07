/*
This Code is written by Abdallaziz Dalloul
This is the Master: this Code recieves a Float value from the Slave

Connection needed
Master Side Slave Side
GND GND
A5 A5 [Clock or SCL]
A4 A4 [Data Line or SDA]
*/

#include<Wire.h>

void setup() {

Wire.begin(); // join i2c bus (address optional for master)
Serial.begin(9600); // start serial for output
}

void loop() {
Wire.requestFrom(2, 20); // request 20 bytes from slave device #2
String string, string1, string2;
do // slave may send less than requested
{
char c = Wire.read(); // receive a byte as character

string = string + c; //Keep saving whatever is comming

string1 = string.substring(0, 8); //slpit String from 0 to 8
string2 = string.substring(9); // Split from 9 to the End of String

} while (Wire.available());
Serial.print("Recieving float as String...");
Serial.println();
Serial.println();
Serial.print("Full String:");
Serial.print(string);

Serial.println();
Serial.print("string 1:");
Serial.print(string1);
Serial.println();

Serial.print("string 2:" );
Serial.print(string2);
Serial.println("");
delay(1000);
Serial.println("");
Serial.println("");
}
