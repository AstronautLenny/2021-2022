#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13 
#define NUMPIXELS 8
int pixelSetA[4] = {0,1,2,3};
int pixelSetB[4] = {7,6,5,4};

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int inputCount = 0;
int solenoidNum;

uint32_t colors[2];
unsigned long previousMillis[2] = {0, 0};
int onPixels[2];
const int solenoids[2] = {8,2};
int timers[2] = {0, 0};
int states[2] = {0, 0};
int durations[2] = {0, 0};

void setup() {
  Serial.begin(9600);
  Wire.begin(0x08); 
  pixels.begin(); 
  pinMode(solenoids[0], OUTPUT);
  pinMode(solenoids[1], OUTPUT);  
}

void loop() {
  Wire.onReceive(receiveData);
  fireSolenoids();
  chooseColorAndProgress();
  setPixels();
}

void receiveData(int howMany) {
  while (Wire.available()) {
    
    //Reset inputCounter after reaching 2 inputs
    if (inputCount == 2) {
      inputCount = 0;
    }

    //Store the first input as the solenoid pin to turn on
    if(inputCount == 0){
      solenoidNum = Wire.read();
      states[solenoidNum] = 1;
      Serial.print("Solenoid ");
      Serial.print(solenoidNum);
    }

    //Store the secound input as the duration to keep the solenoid on
    if(inputCount == 1){
      timers[solenoidNum] = 0;
      durations[solenoidNum] = Wire.read();
    }

    inputCount += 1;
  }
}


void fireSolenoids(){
  //Based on the solenoidPin chosen and duration, fire that solenoid for that amount of time

  //For each solenoid, check if its on, if so add 1 to its timer each second
  for(int i = 0; i < 2; i++){
    if(states[i] == 1){
      if(checkTimePassed(i, 1)){
        timers[i] += 1;
      }
    }

    //If the timer is greater than the duration, turn solenoid off
    if(timers[i] >= durations[i]){
      digitalWrite(solenoids[i], LOW);
      states[i] = 0;    
    }

    //If the timer is less than the duration, turn solenoid on
    else{
      digitalWrite(solenoids[i], HIGH);
    }
  } 
}
 
void chooseColorAndProgress(){

  for(int i = 0; i < sizeof(solenoids)/2; i++){
       if(timers[i] >= 0 && timers[i] <= 1){
      colors[i] = pixels.Color(0, 255, 0);
      onPixels[i] = 1;
    }
    if(timers[i] > 1 && timers[i] <= 2){
      colors[i] = pixels.Color(255, 255, 0);
      onPixels[i] = 2;
    }
    if(timers[i] > 2 && timers[i] <=3){
      colors[i] = pixels.Color(255, 160, 0);
      onPixels[i] = 3;    
    }
    if(timers[i] > 3){
      colors[i] = pixels.Color(255, 0, 0);
      onPixels[i] = 4;
    }
  }   
}

void setPixels(){
  
    if(digitalRead(solenoids[0]) == 1){
      for(int i=0; i<onPixels[0]; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetA[i], colors[0]);
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

    if(digitalRead(solenoids[1]) == 1){
      for(int i=0; i<onPixels[1]; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetB[i], colors[1]);
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

   if(digitalRead(solenoids[0]) == 0){
      for(int i=0; i<4; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetA[i], pixels.Color(0, 0, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

  if(digitalRead(solenoids[1]) == 0){
    for(int i=0; i<4; i++) { // For each pixel... 
      pixels.setBrightness(10);    
      pixels.setPixelColor(pixelSetB[i], pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
  }
}

boolean checkTimePassed(int i, int inputInterval){  
  unsigned long currentMillis = millis();  
  if (currentMillis - previousMillis[i] >= inputInterval*1000) {
    // save the last time you blinked the LED
    previousMillis[i] = currentMillis;
    Serial.println("SECOND PASSED");
    return true;
  }
  return false;
  
}
