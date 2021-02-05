#include <SoftwareSerial.h>
#include <Servo.h>

#define feed_open 13
#define feed_close 12
#define proximity A5

SoftwareSerial sim800(10,11);
Servo arm;

String msg;
bool exec(String);
void deploy(String);
void feed(int,int);

void setup() {
  arm.attach(9);
  sim800.begin(9600);
  Serial.begin(9600); 
  pinMode(feed_open,OUTPUT);
  pinMode(feed_close,OUTPUT);
  pinMode(proximity,INPUT);

  arm.write(0);
  delay(5000);  
  while(exec("AT+CMGF=1\r")){delay(500);}
  delay(1000);
  while(exec("AT+CNMI=1,2,0,0,0\r")){delay(500);}
  delay(1000);

  //deploy("System started successfully\r");
}

void loop() {
  if(sim800.available()){
    msg = sim800.readString();
    Serial.println(msg);
    if(msg.lastIndexOf("feed")!=-1){
      digitalWrite(feed_open,1);
      digitalWrite(feed_close,0);
      feed(120,500);
    }
    else{
      digitalWrite(feed_open,0);
      digitalWrite(feed_close,1);
    }
  }
  //if(digitalRead(proximity)==0){deploy("Proximity alert !!\r");}
  if(Serial.available()){sim800.write(Serial.read());}
}

bool exec(String command){
  String _msg;
  bool ANS=0;
  
  sim800.print(command);
  if(sim800.available()){
    _msg = sim800.readString();
    Serial.println(_msg);
    if(_msg.indexOf("ERROR")!=-1){ANS=1;}
  }

  return ANS;
}

void deploy(String str){
  sim800.println("AT+CMGS=\"+94775917771\"\r");
  if(sim800.available()){Serial.println(sim800.readString());}
  delay(1000);
  
  sim800.print(str);
  if(sim800.available()){Serial.println(sim800.readString());}
  delay(1000);
  
  sim800.write(26);
  if(sim800.available()){Serial.println(sim800.readString());}
  delay(1000);
}

void feed(int range,int t){
  for(int i = 0; i<=range; i++){
    arm.write(i);
    delay(5);
  }
  delay(t);
  for(int j = range; j>=0; j--){
    arm.write(j);
    delay(5);
  }
}
