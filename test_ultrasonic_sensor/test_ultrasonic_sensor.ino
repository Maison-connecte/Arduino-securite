
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Adafruit_NeoPixel.h>

#define SECRET_SSID "Externe"
#define SECRET_PASS "Education2523"


#define LED_PIN 6
#define LED_COUNT 60
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

char ssid[]=SECRET_SSID;
char pass[]=SECRET_PASS;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[]="test.mosquitto.org";
int port=1883;
const char sujet[]="capteur_ultrason";

const long intervale=8000;
unsigned long prevMillis=0;

int pin3=3;
int pinTrigger=5;
unsigned int distance=0;
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(pinTrigger,OUTPUT); 
  digitalWrite(pinTrigger,HIGH);
  pinMode(pin3,INPUT);
  delay(500);
  Serial.print("Connexion au SSID ");
  Serial.println(ssid);
  while(WiFi.begin(ssid,pass)!=WL_CONNECTED){
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Connecte\n");
  Serial.print("Connexion au broker MQTT: ");
  Serial.println(broker);
  if(!mqttClient.connect(broker,port)){
    Serial.println("Echec de connexion au broker\nErreur: "+mqttClient.connectError());
    while (1);
  }
  Serial.println("Connecte au broker\n");

}

void loop() {
  uint32_t color;
  //Serial.print("Distance=");
  digitalWrite(pinTrigger,LOW);
  digitalWrite(pinTrigger,HIGH);
  unsigned long x=pulseIn(pin3,LOW);
  if(x>=50000)
  {
   Serial.println("Invalide"); 
  }
  else{
    distance=x/50;
    mqttClient.beginMessage(sujet);
    mqttClient.print(1);
    mqttClient.endMessage();
   Serial.print(distance);
    Serial.println("cm");
    if (distance<15)
    {
      Serial.println("INTRU DÉTECTÉ");
       color = strip.Color(255, 0, 0);
      delay(200);
    }
    
     else
     {
       Serial.println(" ");
        color = strip.Color(0, 0, 0);
     }
     //sound(distance/4);
     strip.setBrightness(255);
strip.fill(color, 0, 59);
strip.show();
    }
    
}

