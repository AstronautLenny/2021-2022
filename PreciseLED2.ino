#include <Wire.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13 
#define NUMPIXELS 8
int pixelSetA[4] = {0,1,2,3};
int pixelSetB[4] = {7,6,5,4};
//TURN LED ON FOR PRECISE TIME

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t colors[2];

boolean firstReads[2] = {true, true};
int inputCount = 0;
int LEDNum;
int onPixels[2];
unsigned long previousMillis[2] = {0, 0};
unsigned long startMillis[2] = {0, 0};
unsigned long endMillis[2] = {0, 0};
unsigned long totalMillis[2] = {0, 0};
unsigned long runningMillis[2] = {0, 0};

const int LEDS[2] = {8,2};
int states[2] = {0, 0};
int durationsMillis[2] = {0, 0};

void setup() {
  Wire.begin(0x08);
  Serial.begin(9600);
  pinMode(LEDS[0], OUTPUT);
  pinMode(LEDS[1], OUTPUT);
  pixels.begin();
}

void loop() {
  Wire.onReceive(receiveData);
  preciseFire();
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
      LEDNum = Wire.read();
      states[LEDNum] = 1;
        //Serial.println(state);
    }

    //Store the secound input as the duration in millis to keep the solenoid on
    if(inputCount == 1){
      previousMillis[LEDNum] = millis();
      firstReads[LEDNum] = true;
      durationsMillis[LEDNum] = Wire.read();
      //Serial.println(durationMillis);
    }

    inputCount += 1;
  }
}



void preciseFire(){

  for(int i = 0; i < 2; i++){
    if(states[i] == 1){
     if(checkTimePassed(i, durationsMillis[i])){
      digitalWrite(LEDS[i], LOW);
      states[i] = 0;

      //Save time on arduino clock in millis() that the LED turned off (Relative to when the Arduino turned on)
      endMillis[i] = millis();
      Serial.print("End Millis ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(endMillis[i]);


      
      totalMillis[i] = endMillis[i] - startMillis[i];
      Serial.print("LED ");
      Serial.print(i);
      Serial.print(" was on for: ");
      Serial.println(totalMillis[i]);
      Serial.println("");      
    }
    else{
      digitalWrite(LEDS[i], HIGH);
      
     

      //If this is the first time reading the startMillis for the LED
      if(firstReads[i]){
        
        //Save the startMillis as the current time on the arduino clock relative to when the Ardunio turned on
        startMillis[i] = millis();
        Serial.print("Start Millis ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(startMillis[i]);
        firstReads[i] = false;
      }

       //Save each current run time of the LED when it is on after each step through the loop
      //ALL RED LEDS TURN ON AT THE BEGINNING BECAUSE THE MILLIS IS USUALLY GREATER THAN 3000 BUT THE START MILLIS
      runningMillis[i] = millis() - startMillis[i];
      Serial.print("Running Millis ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(runningMillis[i]);
    } 
    } 
  }
}

void chooseColorAndProgress(){

  for(int i = 0; i < sizeof(LEDS)/2; i++){
    
    if(runningMillis[i] >= 0 && runningMillis[i] <= 1000){
      colors[i] = pixels.Color(0, 255, 0);
      onPixels[i] = 1;
    }
    if(runningMillis[i] > 1000 && runningMillis[i] <= 2000){
      colors[i] = pixels.Color(255, 255, 0);
      onPixels[i] = 2;
    }
    if(runningMillis[i] > 2000 && runningMillis[i] <=3000){
      colors[i] = pixels.Color(255, 160, 0);
      onPixels[i] = 3; 
         
    }
    if(runningMillis[i] > 3000){
      colors[i] = pixels.Color(255, 0, 0);
      onPixels[i] = 4;
      
    }
  }   
}

void setPixels(){
  
    if(digitalRead(LEDS[0]) == 1){
      for(int i=0; i<onPixels[0]; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetA[i], colors[0]);
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

    if(digitalRead(LEDS[1]) == 1){
      for(int i=0; i<onPixels[1]; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetB[i], colors[1]);
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

   if(digitalRead(LEDS[0]) == 0){
      for(int i=0; i<4; i++) { // For each pixel... 
        pixels.setBrightness(10);    
        pixels.setPixelColor(pixelSetA[i], pixels.Color(0, 0, 0));
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }

  if(digitalRead(LEDS[1]) == 0){
    for(int i=0; i<4; i++) { // For each pixel... 
      pixels.setBrightness(10);    
      pixels.setPixelColor(pixelSetB[i], pixels.Color(0, 0, 0));
      pixels.show();   // Send the updated pixel colors to the hardware.
    }
  }
}

boolean checkTimePassed(int i, int inputInterval){  
  unsigned long currentMillis = millis();  
  if (currentMillis - previousMillis[i] >= inputInterval) {
    // save the last time you blinked the LED
    previousMillis[i] = currentMillis;
    //Serial.print(inputInterval);
    //Serial.println(" PASSED");
    return true;
  }
  return false; 
}
