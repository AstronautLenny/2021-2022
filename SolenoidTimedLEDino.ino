#include <Wire.h>

const int LEDG1 = 7; // Green LED pin
const int LEDY1 = 8; // Yellow LED pin
const int LEDR1 = 9; // Red LED pin

const int LEDG2 = 4; // Green LED pin
const int LEDY2 = 3; // Yellow LED pin
const int LEDR2 = 2; // Red LED pin

int states[2];
int inputCount = 0;

unsigned long previousMillis[2];
int timers[2];
boolean firstReads[2];
int LEDS[2][3] = {{LEDG1, LEDY1, LEDR1}, {LEDG2, LEDY2, LEDR2}};

void setup() {
  Serial.begin(9600);
  Wire.begin(0x08);  // Joining I2C bus with ADR 0x08

  firstReads[0] = true;
  firstReads[1] = true;

  timers[0] = 0;
  timers[1] = 0;

  previousMillis[0] = 0;
  previousMillis[1] = 0;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(LEDS[i][j], OUTPUT);
      digitalWrite(LEDS[i][j], LOW);
    }
  }
}

void loop() {
  //delay(1000);
  Wire.onReceive(receiveData);
  controlLEDS();
}

boolean timer(int i, const long inputInterval) {
  const long  interval = inputInterval;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis[i] >= interval) {
    // save the last time you blinked the LED
    previousMillis[i] = currentMillis;
    Serial.println("SECOND PASSED");
    return true;
  }
  return false;
}

void setLEDState(int LEDSystem, int LEDOnNum) {
  Serial.println("The current system is ");
  Serial.println(LEDSystem);
  //for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      if(j != LEDOnNum)
        digitalWrite(LEDS[LEDSystem][j], LOW);
    }
  //}
  digitalWrite(LEDS[LEDSystem][LEDOnNum], HIGH);
}


void controlLEDS() {

  for (int i = 0; i < 2; i++) {

    if (states[i] == 1) {
      
      if (firstReads[i]) {
        timers[i] = 0;
        //        digitalWrite(LEDS[i][0], LOW);
        //        digitalWrite(LEDS[i][1], LOW);
        //        digitalWrite(LEDS[i][2], LOW);
      }
      else {
        if (timer(i, 1000)) {
          int LEDOnNum;
          timers[i] += 1;
          Serial.print("Timer");
          Serial.print(i);
          Serial.print(" = ");
          Serial.println(timers[i]);

          if (timers[i] >= 0 && timers[i] <= 5) {
            LEDOnNum = 0;
          }
          else if (timers[i] > 5 && timers[i] <= 7) {
            LEDOnNum = 1;
          }
          else if (timers[i] > 7 ) {
            LEDOnNum = 2;
          }

          setLEDState(i, LEDOnNum);
        }
      }
    
      firstReads[i] = false;
      
    }

    //THIS ISNT BEING CALLED RANDOMLY
    if (states[i] == 0) {
      digitalWrite(LEDS[i][0], LOW);
      digitalWrite(LEDS[i][1], LOW);
      digitalWrite(LEDS[i][2], LOW);
      firstReads[i] = true;
    }
  }
}

void receiveData(int howMany) {

  while (Wire.available()) {
    if (inputCount == 2) {
      inputCount = 0;
    }

    Serial.print("LED SYSTEM");
    Serial.print(inputCount + 1);
    Serial.print(" State = ");
    states[inputCount] = Wire.read();
    Serial.println(states[inputCount]);

    inputCount += 1;
  }
}
