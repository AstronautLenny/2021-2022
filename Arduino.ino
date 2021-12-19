#include <Wire.h> 

const int LED1 = 8; // Red LED pin
const int LED2 = 9; // Green LED pin
int inputCount = 0;
int states[2];
int LEDS[2];

void setup() {
  Serial.begin(9600);
  Wire.begin(0x09);  // Joining I2C bus with ADR 0x08

  inputCount = 0;
  LEDS[0] = LED1;
  LEDS[1] = LED2;

  for(int i = 0; i < sizeof(LEDS)/2;i++){
    pinMode(LEDS[i], OUTPUT);
    digitalWrite(LEDS[i], LOW); 
  }
  Wire.onReceive(receiveData);
}

void loop() {
  // put your main code here, to run repeatedly:
delay(1000); 
}

void receiveData(int howMany) {
    
  while(Wire.available()){
    if(inputCount == 2){
      inputCount = 0;
    }  
    
    Serial.print("LED");
    Serial.print(inputCount + 1);
    Serial.print(" State = ");
    states[inputCount] = Wire.read();
    Serial.println(states[inputCount]);
    
    if(states[inputCount] == 1){
      digitalWrite(LEDS[inputCount], HIGH);
    }
    else{
      digitalWrite(LEDS[inputCount], LOW);
    }
    
    inputCount += 1;
  }
}
