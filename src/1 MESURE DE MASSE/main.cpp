#include <Arduino.h>
#include <Wire.h>
#include <HX711.h>

const int LOADCELL_DOUT_PIN = 16; //GPIO ESP32
const int LOADCELL_SCK_PIN = 4; //GPIO ESP32
HX711 scale;

void setup() {
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
}

void loop() {
   if (scale.is_ready()) 
  {
    int sortiehx = scale.read()/100;// lire la valeur en sortie (170051) et la diviser par 100 (1700)
    int masse= sortiehx -2482;// prendre la valeur précédente et la soustraire à la même valeur (0g)
    int masseeta=(sortiehx -2482)*0.1; // 
    Serial.print("HX711 reading: ");
    Serial.println(scale.read());
    Serial.print(masseeta);Serial.println("g");
    
  } 
  else 
  {
    Serial.println("HX711 not found.");
  }

  delay(1000);
}