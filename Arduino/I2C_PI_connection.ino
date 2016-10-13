#include <Wire.h>
#include <Servo.h>

Servo myservo;

const int address = 4;
unsigned long time;
int number = 0, state = 0;
int minp= 50, maxp = 70;

void setup() {
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), beep, FALLING);

  myservo.attach(7);
  
  Serial.begin(115200);
  //pinMode(LED_BUILTIN, OUTPUT);
  //digitalWrite(LED_BUILTIN, LOW);
  
  Wire.begin(address);
  Wire.onReceive(receiveData);
  //Wire.onRequest(sendData);
}

void loop() {
}

void receiveData(int byteCount) {
  while(Wire.available() > 0) {
    
    char n = Wire.read();
    Serial.println(n);
    
    if (n == 'g') {
      state = 1;
      //digitalWrite(LED_BUILTIN, HIGH);
      tone(12, 300, 300);
      myservo.write(maxp);
    } else if(n == 's' || n == 'x') {
      myservo.write(minp);
    } else {
      state = 0;
    }
  }
}

void beep() {
  int i;
  //time = millis();
  if(state == 1) {
    tone(12, 600, 100);
    Serial.println("L");
    state = 0;
    //for (i = 0; i < 256; i++) {
    //  delayMicroseconds(1000);
    //}
    myservo.write(minp); 
    delay(100);
  }
}
