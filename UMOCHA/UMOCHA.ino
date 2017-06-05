#include <SoftwareSerial.h> 
#define lTrigPin 10
#define lEchoPin 11
#define rTrigPin 12
#define rEchoPin 13
#define piezoPin 9
#define blueTx 6
#define blueRx 5
#define ridPin 7
#define radius 6.5
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(8,DHTTYPE);
String data = "";
float circle = (2 * radius * 3.14) / 100;
float umSpeed = 0;
float dTime = 0;
float sTime = 0;
float eTime = 0;
boolean temp = 0;
int count = 0;
int timer = 0;



SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언

void setup()
{
  pinMode(lTrigPin, OUTPUT);
  pinMode(lEchoPin, INPUT);
  pinMode(rTrigPin, OUTPUT);
  pinMode(rEchoPin, INPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(ridPin, INPUT);
  Serial.begin (9600);
  mySerial.begin(9600); //블루투스 시리얼
  dht.begin();
}
void loop()
{
  getSpeed();
  choempa(lTrigPin,lEchoPin, "left");
  choempa(rTrigPin,rEchoPin, "right");
  if(timer > 100000) {
  getHumidity();
  timer = 0;
  }
  getData();
  delay(1000);
  timer += 100;
  
}

void getSpeed() {
  
  boolean check = digitalRead(ridPin);
    if(check == 1 && temp == 0) {
      sTime = millis();
      temp = 1;
      Serial.println("시작");
    }

    else if(check == 0 && temp == 1 && count > 5) {
      eTime = millis();

      dTime = (eTime - sTime) / 1000;
      umSpeed = (circle / dTime) * 3.6;
      temp = 0;
      count = 0;
      Serial.println("종료");
      Serial.println(umSpeed);
    }

    if(check == 1) {
      count++;
    }
  
}

void getData() {
  while(mySerial.available()) {
    char c = mySerial.read();
    data += c;
    delay(5);
  }
 if(!data.equals("")){
  Serial.println(data);

  if(data == 'breakLock') {  //브레이크 잡기
      
    } else if(data == 'breakUnlock') { //브레이크 해제
      
    }
 }
 data = "";
}

void choempa(int trig,int echo, String s){
  long duration, cm;
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); //10의 -6승초
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH); //장애물까지의 시간
  cm = microsecondsToCentimeters(duration);
  if(timer > 1000) {
    timer = 0;
    mySerial.println(s);
    delay(10);
    mySerial.println(cm);
    delay(10);
  }
  
  if(cm < 50) {
    startBreak();

  }
  
}

void getHumidity(){

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
 
  //mySerial.print("humi");
  //mySerial.print(humidity);
  //mySerial.print("temper");
  //mySerial.print(temperature);
  Serial.println(humidity);
  Serial.println(temperature);
}

void startBreak() {
  piezo();
}

void piezo(){
  tone(piezoPin,7902,300);
  delay(300);
  tone(piezoPin,5000,300);
  delay(300);
  
  
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
