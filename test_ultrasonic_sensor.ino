int pin3=3;
int pinTrigger=5;
unsigned int distance=0;
void setup() {
  Serial.begin(9600);
  pinMode(pinTrigger,OUTPUT); 
  digitalWrite(pinTrigger,HIGH);
  pinMode(pin3,INPUT);
  delay(500);
  
}

void loop() {
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
   // Serial.print(distance);
    //Serial.println("cm");
    if (distance<30)
    {
      Serial.println("INTRU DÉTECTÉ");
    }
    else
    {
      Serial.println(" ");
    }
    }
    
}
