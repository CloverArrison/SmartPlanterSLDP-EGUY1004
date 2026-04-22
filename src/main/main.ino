#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <DHT22.h>

#define moisturePin A0 // 0(wet) < 670 (Dry)
#define lightSensePin A2 // 500 (Dim) > 0 (Bright)
#define tempSensePin 11 // in a unit already C or F
#define fanPin 13
#define pumpPin 12

// Set Values for specific plant
const float LowMoisture = 550;
const float HighMoisture = 300;
const float lowLight = 75;

// Temp sensor object
DHT22 dht22(tempSensePin); 
// LCD object
LiquidCrystal_I2C lcd(0x27,20,4);

void setup() {
  Serial.begin(9600);
  Serial.println("Start");

  //LCD set up
  lcd.init();
  Serial.println("LCD Init");
  lcd.backlight();
  lcd.setCursor(0,0); //Curser starts at the top left
  lcd.print("LCD Setup Done");
  Serial.println("LCD Set Up Done");

  // Input Output Setup
  pinMode(moisturePin, INPUT);
  pinMode(lightSensePin, INPUT);
  pinMode(tempSensePin, INPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);

  // Test Fan and Pump
  Serial.println("Testing Fan and Pump");
  digitalWrite(pumpPin, HIGH);
  delay(500);
  digitalWrite(pumpPin, LOW);
  digitalWrite(fanPin, HIGH);
  delay(500);
  digitalWrite(fanPin, LOW);

  lcd.clear();
  Serial.println("Set Up Done");
}


void loop() {
  // Read sensor values
  float rawLight = analogRead(lightSensePin);
  float rawMoisture = analogRead(moisturePin);
  float tempC = dht22.getTemperature();
  float tempF = dht22.getTemperature()/(9/5)+32;

  // Map sensor values to percent
  float mappedLight = rawLight;//map(rawLight, 512, 0, 100, 0);
  float mappedMoisture = rawMoisture;//map(rawMoisture, 580, 350, 100, 0);

  // Write data to LCD
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Light:     " + String(mappedLight));
  lcd.setCursor(0,1);
  lcd.print("Moisture:  " + String(mappedMoisture));
  lcd.setCursor(0,2);
  lcd.print("Temp (F) : " + String(tempF));
  lcd.setCursor(0,3);
  lcd.print("Temp (C) : " + String(tempC));

  // Run or turn off fan and pump
  handleFan(mappedLight);
  handlePump(mappedMoisture);
  delay(500);
  Serial.println("loop");
}


void handleFan(float light){
  if(light < 60){
    digitalWrite(fanPin, HIGH);
    Serial.println("run fan");
  }
  else digitalWrite(fanPin, LOW);

}

void handlePump(float moisture){
  if(moisture > LowMoisture){
    Serial.println("run pump");
    digitalWrite(pumpPin, HIGH);

  }
  else digitalWrite(pumpPin, LOW);

}
