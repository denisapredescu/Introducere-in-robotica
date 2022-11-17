const int latchPin = 11; // STCP
const int clockPin = 10; // SHCP
const int dataPin = 12; // DS

// declare segments 
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;
int displaySegments[] = {
  segD1, segD2, segD3, segD4
};
// display digits current display
int displayDigits[] = {
  0, 0, 0, 0
};
int currentDisplay = 0;

int currentState = 1; 
// if the current state is 1 means that the DP need to blink, the current position can be changed and the display can be reset
// if the current state is 0 means that the DP need to be on and the current number of the digit can be changed

const int encodingsNumber = 16;
int byteEncodings[encodingsNumber] = {
//A B C D E F G DP 
  B11111100, // 0    
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110  // F
};

// joystick setup
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

byte swState = HIGH;
byte lastSwState = HIGH;

int xValue = 0;
int yValue = 0;

bool joyMoved = false;
const int minThreshold = 400;
const int maxThreshold = 600;

byte reading = 1;
unsigned int lastDebounceTime = 0;
const unsigned int debounceDelay = 50;
const int shortPressTime = 80;
const int longPressTime = 1000;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

// blinking interval
const int blinkTimeOn = 400;
const int blinkTimeOff = 800;
unsigned long lastBlink = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < displayCount; i++) {
    pinMode(displaySegments[i], OUTPUT);
    digitalWrite(displaySegments[i], LOW);
  }
  Serial.begin(9600);
}

void loop() { 
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  checkButtonPressed();
  changeCurrentPosition();
  changeDigitNumber();
  writeNumber(currentDisplay, currentState);
}

// function that checks if the button was pressed and changes the current state
void checkButtonPressed() {
  reading = digitalRead(pinSW);
 
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        pressedTime = millis();
      } else if (swState == HIGH) {
          releasedTime = millis();
          long pressDuration = releasedTime - pressedTime;
          
          if (pressDuration > longPressTime && currentState == 1) {
            resetDisplays();
          } else if (pressDuration > shortPressTime) {
              currentState = !currentState;
          }
      }
    }
  }
  lastSwState = reading;
}

// function that change the current position to other digit (just to neighbors)
void changeCurrentPosition() {
  if (currentState ==  1) {
    if (yValue < minThreshold && !joyMoved) { // LEFT
      if (currentDisplay < 3) {
        currentDisplay = currentDisplay + 1;
      } 
      joyMoved = true;
    }
    if (yValue > maxThreshold && !joyMoved) { // RIGHT
      if (currentDisplay > 0) {
        currentDisplay = currentDisplay - 1;  
      }
      joyMoved = true;
    }
    if (yValue > minThreshold && yValue < maxThreshold) {
      joyMoved = false;
    }
  }
}

// function that increase or decrease the value of the selected digit (moving UP = increase; moving down = decrease)
void changeDigitNumber() {
  if (currentState == 0) {
    if (xValue > maxThreshold && !joyMoved) { // INCREMENT
      if (displayDigits[currentDisplay] < 15) {
        displayDigits[currentDisplay] ++;
      }
      joyMoved = true;
    }
    if (xValue < minThreshold && !joyMoved) { // DECREMENT
      if (displayDigits[currentDisplay] > 0) {
        displayDigits[currentDisplay] --;
      }
      joyMoved = true;
    }  
    if (xValue > minThreshold && xValue < maxThreshold) {
      joyMoved = false;
    }
  }
}

// function that activate and deactivate displays
void activateDisplay(int digit) {
  for(int i = 0; i < displayCount; i++) {
    digitalWrite(displaySegments[i], HIGH);
  }
  digitalWrite(displaySegments[digit], LOW);
}

// function that writes the registers
void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}

void writeNumber(int currentDisplay, int currentState) {
  for (int i = 0; i < displayCount; i++) {
    int digitToWrite = 0;
    if (i == currentDisplay) { 
      if (currentState == 1) { 
        digitToWrite = byteEncodings[displayDigits[currentDisplay]];

        if (millis() - lastBlink > blinkTimeOn) {
          digitToWrite = byteEncodings[displayDigits[currentDisplay]] + 1; 
        } 
        if (millis() - lastBlink > blinkTimeOff) {
          digitToWrite = byteEncodings[displayDigits[currentDisplay]];
          lastBlink = millis();
        }
      } else {
        digitToWrite = byteEncodings[displayDigits[i]] + 1; // the +1 means the DP is on
      }
    } else {
      digitToWrite = byteEncodings[displayDigits[i]];
    }
    writeReg(digitToWrite);
    activateDisplay(i);
    delay(5);
  }
}

// function that resets the digits after long press
void resetDisplays() {
  for (int i = 0; i< displayCount; i++) {
    displayDigits[i] = 0; 
  }
  currentDisplay = 0;
}

