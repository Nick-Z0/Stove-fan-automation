// Author: Nick Papanikolaou, ns.papanikolaou1@gmail.com

#include <max6675.h>
#include <Wire.h>
#include <LCD_I2C.h> // blackhawk 2.30

// Pin definition
const int thermoDO = 35;
const int thermoCS = 33;
const int thermoCLK = 31;
const int relayPin = 7;
const int potPin = A8;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

LCD_I2C lcd(0x27, 16, 2);

// Constants
const long intervalPotRead = 100;
const long intervalTemperatureRead = 1000;
const long intervalDisplayRefresh = 200;
const long intervalRelayUpdate = 5000;

// Variables
int potVal;
int temperature;
int setTemperature;
// For time management
unsigned long previousMillisTemperatureRead = 0; 
unsigned long previousMillisPotRead = 0;
unsigned long previousMillisRelayUpdate = 0;
unsigned long previousMillisDisplayRefresh = 0;

void setup() {

  lcd.begin();
  lcd.backlight();

  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);

  // wait for MAX chip to stabilize
  delay(500);

}

void loop() {
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillisTemperatureRead >= intervalTemperatureRead) {
    previousMillisTemperatureRead = currentMillis;
    Serial.print("C = "); 
    temperature = thermocouple.readCelsius();
    Serial.println(temperature);
  }

  if (currentMillis - previousMillisPotRead >= intervalPotRead) {
    previousMillisPotRead = currentMillis;
    int potVal = analogRead(potPin);
    setTemperature = map(potVal, 0, 1023, 0, 600);
    Serial.println(potVal);
    Serial.println(setTemperature);
  }
  
  if (currentMillis - previousMillisRelayUpdate >= intervalRelayUpdate) {
    previousMillisRelayUpdate = currentMillis;
    if (temperature > setTemperature) {
      digitalWrite(relayPin, HIGH);
    } else {
      digitalWrite(relayPin, LOW);
    }
  }

  if (currentMillis - previousMillisDisplayRefresh >= intervalDisplayRefresh) {
    previousMillisDisplayRefresh = currentMillis;
    // Clear the display
    lcd.clear();  
    // Print temperature on top line
    lcd.setCursor(0,0);
    lcd.print("Set Temp:  ");
    lcd.print(setTemperature);
    lcd.print(" C");
    // Print actual Temperature on bottom line
    lcd.setCursor(0,1);
    lcd.print("Act.Temp:  ");
    lcd.print(temperature);
    lcd.print(" C");
  }
}



