//progetto controllo servoMotor con joystick da remoto 
//telecomando
//23-08-2019

#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>


const int SWpin = 2;
const int vx = A0;
const int vy = A1;

RF24 radio(7,8);
const byte address[6]= "00001";

struct dataStruct{
  int x;
  int y;
  int s;
}myData;

void setup() {

  Serial.begin(9600);
  pinMode(SWpin,INPUT);
  digitalWrite(SWpin, HIGH);
 
  radio.begin();//fa partire la trasmittente
  radio.openWritingPipe(address);//diciamo qual è il canale
  //radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();//per mettere in trasmissione

  myData.x = 507;
  myData.y = 520;
  myData.s = 1;
}

void loop() {
  myData.s = digitalRead(SWpin);
  myData.x = analogRead(vx);
  myData.y = analogRead(vy);
  Serial.println(myData.x);
  Serial.println(myData.y);
  Serial.println("\n");
  radio.write(&myData,sizeof(myData));
  
  delay(1);
 
}
