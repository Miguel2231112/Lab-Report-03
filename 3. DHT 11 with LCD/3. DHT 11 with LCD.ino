#include <Adafruit_Sensor.h> // includes the HC-SR04/Ultrasonic Sensors library
#include <DHT.h> // includes the DHT-11's library
#include <DHT_U.h> // includes additional DHT library

#include <LiquidCrystal.h>// includes the LCD library

#define DHTPIN 7     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
// #define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //sets pin number for LCD 

byte degreesChar[8] = { B00111,B00101,B00111,B00000,B00000,B00000,B00000, B00000};// Creates the celcius degree symbol as a custom character
// Creates the temperature symbol in LCD using 

/*
Main Theme : 
     The goal of this program is to show the functionality of the sensor "DHT-11" while taking advantage of showing the values in the LCD
*/
void setup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name); // Print temperature sensor type
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version); // Print temperature sensor version
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id); // Print temperature sensor ID
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C")); // Print temperature sensor max read value for temperature with symbol
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C")); // Print temperature sensor min read value for temperature with symbol
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C")); // Print temperature sensor resolution for temperature with symbol
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name); // Print temperature sensor type
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version); // Print temperature sensor version
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id); // Print temperature sensor ID
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%")); // Print temperature sensor max read value for temperature
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%")); // Print temperature sensor min read value for temperature
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%")); // Print temperature sensor resolution 
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
  
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  lcd.createChar(0, degreesChar); // prints custom degree character in LCD using byte
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event; //encapsulates sensor data from any type of sensor.
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
