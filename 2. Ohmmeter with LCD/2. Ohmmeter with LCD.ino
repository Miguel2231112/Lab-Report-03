#include <LiquidCrystal.h>
float volt, Rtest, Rrefer = 1000; // used float to approximate analog and continuous values because they have greater resolution than integers, and to give Rrefer an amount of 1000 ohms for reference
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte omega[8] = {B01110, B11011, B10001, B10001, B11011, B01110, B11011, B0000}; //draws the Omega symbol for resistance
/*
Main Theme : 
     The goal of this program is to calculate the values of resistance using the bit value converted to voltage that will be converted to resistance right after and the values will be displayed in the LCD
*/
void setup() {
  lcd.begin(16, 2); //initializes the LCD
  lcd.createChar(1, omega); //creates the symbol for resistance from "byte omega[8]"
  lcd.setCursor(0,0);// sets "Resistance : " starting on the 1st block and 1st row ( optional because even without this it sets it to the default position which is (0,0) ) 
  lcd.print("Resistance : ");
}

void loop() {
  volt = analogRead(A0); // sets the bit value with a label that will later be converted to volts
  volt = volt * (5.0 / 1023.0); // converts bit value to voltage to use for the conversion of resistance
  Rtest = (5 - volt) * Rrefer / volt; //coverts the following values above to resitance that will later be put in the LCD
  
  if((Rtest) < 1000){ //if statement is for optimization that implies that if the resistance value is less than 1000 it will show the whole number without other notaions such as kilos, mega, etc.
  lcd.setCursor(0 , 1); //sets the value on the second row first block and 2nd Row
  lcd.print(Rtest); //Prints the resitance value in the LCD
  lcd.print(" "); //Prints space to see value clearly
  lcd.write(1); //used lcd.write to print the character that was created from "lcd.createChar(1, omega);"
  }
  
  else if ((Rtest) > 1000){ //if statement is for optimization that implies that if the resistance value is more than 1000 it will show the whole number with the kilo notation
   lcd.setCursor(0 , 1); //sets the value on the second row first block and 2nd Row
   lcd.print(Rtest/1000); //Prints the resitance value divided by a thousand to show the "kilo" value in the LCD 
   lcd.print(" k"); //Prints space and k to indicate value have passed thousands
  lcd.write(1); //used lcd.write to print the character that was created from "lcd.createChar(1, omega);"
  }
} 