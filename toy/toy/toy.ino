 #include <Servo.h>
Servo servo;
///////////////////////////////////
#include <UnoWiFiDevEd.h>
#include <Wire.h>
#define CONNECTOR "rest"
#define SERVER_ADDR "158.108.165.223"
char readAddr[] = "/data/AssassinZEED/toyison";
char readAddrMan[] = "/data/AssassinZEED/toyman";
String writeAddr = "/data/AssassinZEED/toyman/set/false";
//char str[30];
CiaoData data;
////////////////////////////////////////////////////
#include <pt.h>
#define PT_DELAY(pt, ms, ts)\
  ts = millis();\
  PT_WAIT_WHILE(pt, millis()-ts < (ms));
struct pt pt_toy;
#define TRIG_PIN 7 
#define ECHO_PIN 8
int duration,distance,count1,count2,i;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  PT_INIT(&pt_toy);
  Ciao.begin();
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  servo.attach(9);
}

void loop() {
  toy(&pt_toy);
  // put your main code here, to run repeatedly:

}
PT_THREAD(toy(struct pt* pt)){
  static uint32_t ts;
  PT_BEGIN(pt);
  while(1){
    distance=Ultra();
    data = Ciao.read(CONNECTOR, SERVER_ADDR, readAddr);
   Serial.println(String(data.get(2)));//-------------
    if(String(data.get(2))=="false"){//ไม่ได้auto
     data = Ciao.read(CONNECTOR, SERVER_ADDR, readAddrMan);/////////////////////
     Serial.println(data.get(2));//-------------
    // manOrder = String(data.get(2));
   
     if(String(data.get(2))=="true"){
       i=0;
       for( ;i<=3;i++){
        servo.write(250);
        PT_DELAY(pt,1000, ts);
         servo.write(10);
         PT_DELAY(pt,1000, ts);
         Serial.println("1round");
       }
       data = Ciao.write(CONNECTOR, SERVER_ADDR, writeAddr);
     }   
     }
     else if(String(data.get(2))=="true"){//auto
      { Serial.println(distance);
        if(distance<=10&&distance>=0){
         
      do{
       // i=0;
        //for(;i>=3;i++){
        servo.write(250);
        PT_DELAY(pt, 1000, ts);
         servo.write(10);
         PT_DELAY(pt, 1000, ts);
        //}
        //servo work จนกว่าจะไป // function เปิดปิด ultr
        distance=Ultra(); 
        if(!String(data.get(2)))
          break;
        Serial.println(distance);
      }while(distance<=10);
     }
        }
     }
  }
  PT_END(pt);
}
int Ultra(){
    digitalWrite(TRIG_PIN, LOW);
    delay(500);
    digitalWrite(TRIG_PIN, HIGH);
    delay(500);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN,HIGH);
    int distanceCm = duration / 29.1 / 2 ;
    return distanceCm;
  }
