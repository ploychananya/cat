#include <UnoWiFiDevEd.h>
#include <Wire.h>
#define CONNECTOR "rest"
#define SERVER_ADDR "158.108.165.223"
//String writeAddrplus = "/cat/add";
//String writeAddrmin = "/cat/minus";

char writeAddrplus[] = "/data/AssassinZEED/count/set/plus";
char writeAddrmin[] = "/data/AssassinZEED/count/set/minus";
CiaoData data;
#include <pt.h>
#define PT_DELAY(pt, ms, ts)\
  ts = millis();\
  PT_WAIT_WHILE(pt, millis()-ts < (ms));
struct pt pt_task2;
#define TRIG_PIN1 10 // outhome
#define ECHO_PIN1 11          //มี 2ตัว
#define TRIG_PIN2 12 // inhome
#define ECHO_PIN2 13
int duration1, distanceMeter1,duration2, distanceMeter2,count1,count2;
void setup() {
  Serial.begin(9600);
  PT_INIT(&pt_task2);
  ////////////////////////////////////////////////////////////////////////
  Ciao.begin();
  ////////////////////////////////////////////////////////////////////////
  pinMode(TRIG_PIN1,OUTPUT);
  pinMode(ECHO_PIN1,INPUT);
  pinMode(TRIG_PIN2,OUTPUT);
  pinMode(ECHO_PIN2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  task2(&pt_task2);
  ////////////////////////////////////////////////////////////////////////
}
PT_THREAD(task2(struct pt* pt)){
  static uint32_t ts;
  PT_BEGIN(pt);
  while(1){
    digitalWrite(TRIG_PIN1, LOW);
    PT_DELAY(pt, 500, ts);
    digitalWrite(TRIG_PIN1, HIGH);
    PT_DELAY(pt, 500, ts);
    digitalWrite(TRIG_PIN1, LOW);
    duration1 = pulseIn(ECHO_PIN1,HIGH);
    distanceMeter1 = duration1 / 29.1 / 2 ;
digitalWrite(TRIG_PIN2, LOW);
  PT_DELAY(pt, 500, ts);
  digitalWrite(TRIG_PIN2, HIGH);
  PT_DELAY(pt, 500, ts);
  digitalWrite(TRIG_PIN2, LOW);
  duration2 = pulseIn(ECHO_PIN2,HIGH);
  distanceMeter2 = duration2 / 29.1 / 2;
 if(distanceMeter1<distanceMeter2){// outside
  if(distanceMeter1<0){
    Serial.print("1ERROR!");
    Serial.println(distanceMeter1);
    count1=0;
    count2=0; 
    }
  else if 
  (distanceMeter1 <= 3&count1!=1){// ต้องมั่นใจว่าเข้าใกล้จริงๆ
    //ส่งไปว่าminus
    Serial.print("fromout ");
    Serial.println(distanceMeter1);
    count1++;
    count2=0;
  }
    else if(distanceMeter1 <= 3&count1==1)
    {data  = Ciao.write(CONNECTOR, SERVER_ADDR, writeAddrplus);
    Serial.println("openplus");
    PT_DELAY(pt, 2000, ts);
    data  = Ciao.write(CONNECTOR, SERVER_ADDR,"/data/AssassinZEED/count/set/-");
    Serial.println(data.get(2));
    count1=0;
    count2=0; 
    
    }
    }  
 else if(distanceMeter2<distanceMeter1){   
    if(distanceMeter2<0){
    Serial.print("2ERROR!");
    Serial.println(distanceMeter2);
    count1=0;
    count2=0; 
    }
    else if(distanceMeter2 <= 3&count2!=1){
      count2++;
      count1=0;
      Serial.print("inside ");
      Serial.println(distanceMeter2);
    }
    else if(distanceMeter2 <= 3&count2==1){
    data = Ciao.write(CONNECTOR, SERVER_ADDR, writeAddrmin);
    Serial.println("openminus");//เซฮเวอร์โวทำงาน
    data  = Ciao.write(CONNECTOR, SERVER_ADDR,"/data/AssassinZEED/count/set/-");
    PT_DELAY(pt, 2000, ts);
    Serial.println(data.get(2));
    count2=0;
    count1=0;
    }
    }
  }
  PT_END(pt);
}

