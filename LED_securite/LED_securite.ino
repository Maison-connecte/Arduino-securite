//Auteur: Dylan Lévesque
//Date de création: 23 mars 2023
//Description: Code permettant au microcontrôleur de communiquer avec un capteur à ultrason et une bande DEL, avec envoi sur broker par MQTT
//Le capteur reçoit une valeur de distance et dépendemment de si elle déscend en dessous de 15 cm, envoyer une donnée par MQTT et allumer la DEL en rouge
#include <ArduinoMqttClient.h>
#include <WiFiNINA.h>
#include <Adafruit_NeoPixel.h>



//Broche qui envoie les données servant à contrôler la bande LED
#define LED_PIN 6
//Nombre de LED sur la bande
#define LED_COUNT 60
//Variable de la bande LED
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//SSID et mot de passe du réseau WiFi
char ssid[]="Externe";
char pass[]="Education2523";

//Variable de connexion au WiFi
WiFiClient wifiClient;
//Variable de connexion au broker MQTT
MqttClient mqttClient(wifiClient);

//Informations du broker et du topic
const char broker[]="test.mosquitto.org";
int port=1883;
const char sujet[]="capteur_ultrason";


const long intervale=8000;

unsigned long prevMillis=0;

//Couleur de la LED
uint32_t    red = strip.Color(255, 0, 0);

//Broches du capteur à ultrason
int pin_capteur=3;
int pin_trigger=5;

//Distance perçue par le capteur à ultrason
unsigned int distance=0;
void setup() {
  Serial.begin(9600);
  //Initialiser la bande LED éteinte
  strip.begin(); 
  strip.show(); 
  //Initialisation du capteur à ultrason
  pinMode(pin_trigger,OUTPUT); 
  digitalWrite(pin_trigger,HIGH);
  pinMode(pin_capteur,INPUT);
  delay(500);

  //Connexion au réseau WiFi
  Serial.print("Connexion au SSID ");
  Serial.println(ssid);
  while(WiFi.begin(ssid,pass)!=WL_CONNECTED){
    Serial.print(".");
    delay(5000);
  }
  Serial.println("Connecte\n");

  //Connexion au broker MQTT
  Serial.print("Connexion au broker MQTT: ");
  Serial.println(broker);
  if(!mqttClient.connect(broker,port)){
    Serial.println("Echec de connexion au broker\nErreur: "+mqttClient.connectError());
    while (1);
  }
  Serial.println("Connecte au broker\n");
  mqttClient.setKeepAliveInterval(15);
}

void loop() {
  //Envoi de messages "keep alive" pour éviter que le Arduino soit déconnecté du broker MQTT
  mqttClient.poll();

  //Réception des valeurs du capteur à ultrason
  digitalWrite(pin_trigger,LOW);
  digitalWrite(pin_trigger,HIGH);
  unsigned long x=pulseIn(pin_capteur,LOW);
  
  //Valeur trop grande pour être valide
  if(x>=50000)
  {
   Serial.println("Invalide"); 
  }
  //Conversion en cm
  else{
    distance=x/50;
    
   Serial.print(distance);
    Serial.println("cm");
    //Envoi d'un 1 au topic lorsqu'un mouvement est détecté
    if (distance<15)
    {
      envoyerMessage("1");
     strip.setBrightness(50);
     //Allumage de la LED
    strip.fill(red, 0, 59);
    strip.show();
     Serial.println("INTRU DÉTECTÉ");
    }
    else{
      strip.clear();
      strip.show();
    }
  
    
 

    }
    
}
//Envoi de message au topic
void envoyerMessage(String valeur){


    mqttClient.beginMessage(sujet);
    mqttClient.print(valeur);
    mqttClient.endMessage();
      
       
}
