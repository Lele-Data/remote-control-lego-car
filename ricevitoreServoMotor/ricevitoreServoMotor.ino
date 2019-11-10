//progetto controllo servoMotor con joystick da remoto
//ricevente
//23-08-2019

#include <Servo.h>
#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>

const int servoAttach = 9;
const int piezoPin = 3;
const int controlPin1 = 5;
const int controlPin2 = 4;
const int enablePin = 6;

RF24 radio(7,8);
const byte address[6]= "00001";

struct dataStruct{
  int x;
  int y;
  int s;
}myData;

Servo myServo;
int pos = 90;

int servoPos(int v){
  int val;
  double m;
  if(v>=520){
    m = 90./(1023.- 520.); 
    val = m*v;
    return val;
  }
  m = 90./520.;
  val = m*v;
  return val;
}

int setMotorSpeed(int v){
  int val;
  double m;
  if(v>=507){
    m = 255/(1023.- 507.); 
    val = m*(v-507.);
    return val;
  }
  m = 255./507.;
  val = m*v;
  return 255-val;
}


void setup() {

  Serial.begin(9600);
 
  myServo.attach(servoAttach);
  myServo.write(pos);

  pinMode(enablePin,OUTPUT);
  pinMode(controlPin1,OUTPUT);
  pinMode(controlPin2,OUTPUT);
  pinMode(piezoPin,OUTPUT);
  digitalWrite(enablePin,LOW);
  noTone(piezoPin);

  radio.begin();//fa partire la trasmittente
  radio.openReadingPipe(0,address);//diciamo qual è il canale
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();//per mettere in ricezione

  myData.x = 507;
  myData.y = 520;
  myData.s = 1;
  delay(3000);
}

void loop() {
  
  if(radio.available()){

    radio.read(&myData,sizeof(myData));
    
    int SWvalue = myData.s;
    int xValue = myData.x;
    int yValue = myData.y;

    Serial.println(xValue);
    Serial.println(yValue);
    Serial.println("\n");
  
    if(xValue >= 507){
      digitalWrite(controlPin1,HIGH);
      digitalWrite(controlPin2,LOW);
      analogWrite(enablePin,setMotorSpeed(xValue));
    }else{
      digitalWrite(controlPin2,HIGH);
      digitalWrite(controlPin1,LOW);
      analogWrite(enablePin,setMotorSpeed(xValue));
    }
  
    if(SWvalue==0) {
      tone(piezoPin,500);
    }else{
      noTone(piezoPin);
    }
    
    myServo.write(servoPos(yValue));
  }
}
