#include <UnoWiFiDevEd.h>
#include <Wire.h>
#include <Servo.h>
#define CONNECTOR "rest"
#define SERVER_ADDR "158.108.165.223"
char readAddr[] = "/data/AssassinZEED/doorlock";
//String writeAddrplus = "/data/AssassinZEED/countcat/set/plus";
//String writeAddrmin = "/data/AssassinZEED/countcat/set/minus";
//String writeAddr = "/data/AssassinZEED/countcat/set/";
Servo servo;
String order;
CiaoData data;

#include <pt.h>
#define PT_DELAY(pt, ms, ts)\
  ts = millis();\
  PT_WAIT_WHILE(pt, millis()-ts < (ms));
struct pt pt_lock;

void setup() {
  Serial.begin(9600);
  Ciao.begin();
  servo.attach(5);
  lock(&pt_lock);
}

void loop() {
  lock(&pt_lock);
}

PT_THREAD(lock(struct pt* pt)){
  static uint32_t ts;
  PT_BEGIN(pt);
  while(1){
    data = Ciao.read(CONNECTOR, SERVER_ADDR, readAddr);
    order=String(data.get(2));
  if(order=="true"){
    servo.write(140);
    PT_DELAY(pt,2000, ts);
  }
  else if(order=="false"){
    servo.write(30);
    
  }
    
  }
  PT_END(pt);
}
  
  

