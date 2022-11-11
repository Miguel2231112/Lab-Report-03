#include <LiquidCrystal.h>//includes the LCD library
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
/*
Main Theme : 
     The goal of this program is to calculate the voltage using the bit value that will later be displayed in the LCD
*/
void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  // VOLTAGE:
  int val = analogRead(A0);
  Serial.print("Bit Value: ");
  Serial.print(val);

  float volt = val/204.6;

  Serial.print(" Voltage: ");
  Serial.println(volt);

  lcd.setCursor(0, 0);
  lcd.print("Bit Value: ");
  lcd.print(val);
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("Voltage: ");
  lcd.print(volt);
  lcd.print(" V");
}

