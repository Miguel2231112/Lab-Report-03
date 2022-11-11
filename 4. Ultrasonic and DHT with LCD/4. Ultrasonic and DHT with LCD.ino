#include <Adafruit_Sensor.h> // includes the HC-SR04/Ultrasonic Sensors library
#include <DHT.h> // includes the DHT-11's library
#include <DHT_U.h> // includes additional DHT library
#include <LiquidCrystal.h> // includes the LCD library

#define trigPin 10 //defines pin 10 as a trigger 
#define echoPin 13 //defines pin 13 as an echo
#define DHTPIN 6 //data for the temp sensor
#define DHTTYPE    DHT11 //type of DHT "DHT-11"

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; // creates variables for pin numbers in LCD
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //sets pin number for LCD 

int tempPB; //sets a variable to switch between temp to dist (vice-versa) as a push button
int distPB; //sets a variable to switch between dist to temp (vice-versa) as a push button

unsigned long previousMillis = 0; // will store last value of previousMillis
const long interval = 500; //sets an interval of 500 milliseconds
//Sets the degree symbol for Celcius in the eighth spot of the LCD
byte celcius[8] = {B00111, B00101, B00111, B00000, B00000, B00000, B00000, B00000}; //

float duration, distance;
/*
Main Theme : 
     The goal of this program is to show the functionality of the sensors "HC-SR04" and "DHT-11" while taking advantage of showing the values in the LCD with the added interaction of pushbuttons to switch displays.
*/
void setup() {
  Serial.begin(9600);
  
  pinMode(9, INPUT_PULLUP); // Temperature and Humidity DHT
  pinMode(8, INPUT_PULLUP); // Ultrasonic HC-SR04
  // Temperature and Humidity
  dht.begin(); //begins the sequence of the Temperature and Humidity Sensor
  // Ultrasonic
  pinMode(trigPin, OUTPUT); // sets up pin 10 "trigPin" as OUTPUT to trigger the ultrasonic sound pulses.
  pinMode(echoPin, INPUT); // sets up pin 13 "echoPin" as n input because it produces a pulse when the reflected signal is "received".
  // LCD
  lcd.begin(16, 2); //Begins LCD setup
  lcd.createChar(0, celcius); //creates the symbol for celsius/degrees
}

void loop() {
  // Push Buttons
  int tempPB = digitalRead(9); //sets "digitalRead(9)" as tempPB
  int distPB = digitalRead(8); //sets "digitalRead(9)" as distPB

  if (tempPB == 0) { // if the value of pin 9 reads 0 LCD (value can switch if button for tempPB is pressed "digitalRead(9)")
    tempPB = 1; 
    distPB = 0;
    lcd.clear();
  }

  if(distPB == 0) { // if the value of pin 9 reads 0 LCD (value can switch if button for distPB is pressed "digitalRead(8)")
    tempPB = 0;
    distPB = 1;
    lcd.clear();
  }
  
  unsigned long currentMillis = millis(); //This means that other code can run at the same time without being interrupted

  // Temperature and Humidity
  if (tempPB == 1) {
    if (currentMillis - previousMillis >= interval) {//uses "if" statement to make a condition that if current millis is subtracted by the previous one is more than or equal to interval(500) which will then cancel the delay and optimize
      previousMillis = currentMillis;  //then previousmillis will equal the current one

      delay(delayMS); //delay
      sensors_event_t event;
      // Get temperature event and print its value.
  dht.temperature().getEvent(&event); //Program from the library that configures the temperature
  if (isnan(event.temperature)) { //if value is NaN "Not a Number" print the next line
    Serial.println(F("Error reading temperature!")); // Prints "Error reading temperature!" if temperature is unreadable / troubleshoot function
  }
      else { //if the if statement does not verify the situation then it willl print the following :
    Serial.print(F("Temperature: ")); // Prints the Temperature in the serial monitor
    Serial.print(event.temperature); 
    Serial.println(F("°C"));

    lcd.setCursor(0, 0); // Prints the following texts in the first layer (0, 0)
    lcd.print("Temp: "); // Prints the label for Temperature in the 16x2 LCD on the first layer
    lcd.print(event.temperature); // Prints the value for Temperature 
    lcd.write(byte(0));
    lcd.print("C");//Prints the Celcius symbol to indicate Temperature after the value
    lcd.print("     ");//Prints spaces
      }

       // Get humidity event and print its value.
      dht.humidity().getEvent(&event); //Program from the library that configures the humidity
  if (isnan(event.relative_humidity)) { //if value is NaN "Not a Number" print the next line
    Serial.println(F("Error reading humidity!")); // Prints "Error reading humidity!" if humidity is unreadable / troubleshoot function
  }
  else { //if the if statement does not verify the situation then it willl print the following :
    Serial.print(F("Humidity: ")); // Prints the Label for Humidity in the serial monitor
    Serial.print(event.relative_humidity); // Prints the Humidity in the serial monitor
    Serial.println(F("%")); //prints percentage symbol after the value

    lcd.setCursor(0, 1); // Prints in the 16x2 LCD on the second layer (0, 1)
    lcd.print("Humid: "); // Prints the label for Humidity in the 16x2 LCD on the second layer
    lcd.print(event.relative_humidity); // Prints the value for Humidity
    lcd.print("%"); // Prints the percentage symbol in the 16x2 LCD on the second layer following the amount of humidity
    lcd.print("     "); //Prints spaces
      }
    }
  }
  // Ultrasonic
  if (distPB == 1) {
    digitalWrite(trigPin, LOW); // Clears the trigPin
    delayMicroseconds(2); //delays by 2 microseconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, LOW); //turns of trigPin but later turns on because function is in the loop

    duration = pulseIn(echoPin, HIGH); //Reads a pulse (either HIGH or LOW) on a pin.

    distance = (duration/2)*0.0343; // Calculation for distance using the speed of sound for th ereviever to read the right value

    Serial.print("Distance = "); // Prints Distance in the serial monitor to check value before printing in the LCD
    
    if (distance >=400 || distance <= 2) { // if statement is used in this case to state the maximum and minimum value of distance in cm, and if value goes past or below this it will print "Out of range"
      Serial.println("Out of range!"); //prints "Out of range" in the serial monitor if goes past or below the value in the if statement
      lcd.setCursor(0,0); //sets starting point on first row first block
      lcd.print("Out of range!"); //prints "Out of range" in the LCD if goes past or below the value in the if statement
      lcd.setCursor(0,1); // sets next starting point on the second layes first block
      lcd.print("       "); // prints spaces 
    } else {
      Serial.print(distance); // prints calculated distance in the serial monitor to check before putting in the LCD
      Serial.println(" cm"); // labels the value as cm
      if (currentMillis - previousMillis >= interval) {
      //uses "if" statement to make a condition that if current millis is subtracted by the previous one is more than or equal to interval(500) which 
      //will then cancel the delay and optimize
        previousMillis = currentMillis; // previousmillis will equal the current one
        lcd.setCursor(0,0); //sets starting point on first row first block
        lcd.print("Distance:    ");// prints "Distance:" in the LCD to label the following values
        lcd.setCursor(0,1);// sets next starting point on the second layes first block
        lcd.print(distance); //prints the range of distance sensor is detecting
        lcd.print(" cm"); // labels value as cm
        lcd.print("     "); //prints spaces
      }
    }
  }
}
