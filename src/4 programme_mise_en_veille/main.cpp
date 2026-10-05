#include <Arduino.h>

#define uS_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP  5   // DUREE PHASE D'ENDORMISSEMENT EN SECONDES

RTC_DATA_ATTR int bootCount = 0;

void print_wakeup_reason(){
   esp_sleep_wakeup_cause_t source_reveil;  // CONFIGURATION REVEIL PAR TIMER
 }

void setup(){
   Serial.begin(115200);
   pinMode(21,OUTPUT); // LED INDICATEUR MODE VEILLE
      pinMode(32,OUTPUT); // SIGNAL CMD COMMANDE SWITCH EXTINCTION HX711 ET PDT
 }

void loop(){

  /////////////// MESURES ET TRANSMISSION PENDANT REVEIL////////////////////////////////
   digitalWrite(21, HIGH);// allume LED CARTE lorsque ESP32 réveillé
    digitalWrite(32, HIGH);// commande CMD=1 ALLUMAGE HX711 ET PDT
   delay(10000);// DUREE SIMULEE PHASE MESURES ET TRANSMISSIONS
   
 
   
   ////////////ENDORMISSEMENT ESP32 ET EXTINCTION HX711 ET PDT////////////////////////////////
      //Configuration du timer
   esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // REGLAGE DUREE ENDORMISSEMENT
 

   //Rentre en mode Deep Sleep
  
   digitalWrite(21, LOW);// éteint LED CARTE lorsque ESP32 endormi
   digitalWrite(32, LOW);// commande CMD=0 EXTINCTION HX711 ET PDT
   esp_deep_sleep_start(); // ENDORMISSEMENT ESP32
   
 

}