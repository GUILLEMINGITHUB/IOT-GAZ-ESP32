#include <Arduino.h>
#include "secrets.h"

/////////CONFIG MESURE DE MASSE///////////////
      #include <Wire.h>
      #include <HX711.h>
      #include <WiFi.h>
      const int LOADCELL_DOUT_PIN = 16; //GPIO ESP32
      const int LOADCELL_SCK_PIN = 4; //GPIO ESP32
      HX711 scale;
//////CONFIG DEEP SLEEP/////////////
      #define uS_TO_S_FACTOR 1000000
      #define TIME_TO_SLEEP  5   // DUREE PHASE D'ENDORMISSEMENT EN SECONDES
      
      RTC_DATA_ATTR int bootCount = 0;

//// CONFIG WIFI//////////
      #include <HTTPClient.h>
      unsigned long lastTime = 0;
      unsigned long timerDelay = 10000;
      
      
      // Domain Name with full URL Path for HTTP POST Request
      const char* serverName = "http://api.thingspeak.com/update";
      // Service API Key
     


void print_wakeup_reason(){
   esp_sleep_wakeup_cause_t source_reveil;  // CONFIGURATION REVEIL PAR TIMER
 }

String massestring="";
String Pourcentagestring="";


void setup(){
       Serial.begin(115200);
       WiFi.begin(ssid, password);
      Serial.println("Connecting");
      while(WiFi.status() != WL_CONNECTED) {
                                          delay(500);
                                          Serial.print(".");
                                          }
      Serial.println("");
      Serial.print("Connected to WiFi network with IP Address: ");
      Serial.println(WiFi.localIP());
 
      Serial.println("Timer set to 10 seconds (timerDelay variable), it will take 10 seconds before publishing the first reading.");

       scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); //BROCHES HX711
       pinMode(21,OUTPUT); // LED INDICATEUR MODE VEILLE
       pinMode(32,OUTPUT); // SIGNAL CMD COMMANDE SWITCH EXTINCTION HX711 ET PDT
       pinMode(34,INPUT); // SIGNAL CMD COMMANDE SWITCH EXTINCTION HX711 ET PDT
 }

void loop(){

  /////////////// MESURES ET TRANSMISSION PENDANT REVEIL////////////////////////////////
  
  
        digitalWrite(32, HIGH); // commande CMD=1 ALLUMAGE HX711 ET PDT
        delay(2000);            // temps de mise en service HX711
                /////MESURE MASSE//////////////
                    if (scale.is_ready()) 
                {
                  int sortiehx = scale.read()/100;// lire la valeur en sortie (170051) et la diviser par 100 (1700)
                  int masse= sortiehx -2690;// prendre la valeur précédente et la soustraire à la même valeur (0g)
                  int masseeta=(sortiehx -2690)*0.1; // 
                  Serial.print("HX711 reading: ");
                  Serial.println(scale.read());
                  Serial.print(masseeta);Serial.println("g");
                   massestring= String(masseeta,DEC); // CONVERSION MASSE EN CHAINE DE CARACTERE POUR TRANSMISSION WIFI
                 } 
                else 
                {
                  Serial.println("HX711 not found.");
                }
                /////MESURE NIVEAU BATTERIE///////////////
       
                 analogReadResolution(10);
                 int  Nmesure = analogRead(34); // lit la broche d'entrée
                  float VBAT=Nmesure*0.004398;
                  int P = map(Nmesure,800,1023,0,100);
                  
                  Serial.print("Nmesure");
                  Serial.println(Nmesure);//renvoie la valeur envoyée sur la broche 35 de l'Arduino UNO
                  Serial.print("Vbat=");
                  Serial.println(VBAT);//renvoie la valeur envoyée sur la broche 35 de l'Arduino UNO
                  Serial.print("pourcentage");
                  Serial.println(P);
                  Pourcentagestring= String(P,DEC); // CONVERSION POURCENTAGE BATTERIE EN CHAINE DE CARACTERE POUR TRANSMISSION WIFI
                
   ////////////////transmission////////////////////////
            
                    if(WiFi.status()== WL_CONNECTED){
                      WiFiClient client;
                      HTTPClient http;
                      // Your Domain name with URL path or IP address with path
                      http.begin(client, serverName);
                      
                      // Specify content-type header
                      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
                      // Data to send with HTTP POST
                      String httpRequestData = "api_key=" + apiKey + "&field1=" + massestring + "&field2=" + Pourcentagestring;           
                      // Send HTTP POST request
                      int httpResponseCode = http.POST(httpRequestData);
                                                           
                      Serial.print("HTTP Response code: ");
                      Serial.println(httpResponseCode);
                      // Free resources
                      http.end();
                    }
                  else {
                      Serial.println("WiFi Disconnected");
                    }
         
    
   ////////////ENDORMISSEMENT ESP32 ET EXTINCTION HX711 ET PDT////////////////////////////////
      //Configuration du timer
         esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR); // REGLAGE DUREE ENDORMISSEMENT
 
       //Rentre en mode Deep Sleep
         digitalWrite(32, LOW);// commande CMD=0 EXTINCTION HX711 ET PDT
         esp_deep_sleep_start(); // ENDORMISSEMENT ESP32
      
 }