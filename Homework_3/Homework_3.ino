// declare all the joystick pins
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

bool commonAnode = false; 
const int segSize = 8;
const int noOfMovements = 4;
const byte defaultState = LOW;
byte state = HIGH;
byte swState = HIGH;
byte lastSwState = HIGH;
int xValue = 0;
int yValue = 0;

// default state
int currentState = 1;
// starts with dp high
int currentSegment = 7;
int lastSegment = 7;

// blinking interval
const int blinkTime = 250;
unsigned long lastBlink = 0;
byte blink = LOW;

byte reading = 1;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

const int shortPressTime = 800;
const int longPressTime = 1000;
unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;

bool joyMoved = false;
int minThreshold = 400;
int maxThreshold = 600;

int segments[segSize] = { 
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};

byte segmentStates[segSize] = {
  defaultState, defaultState, defaultState, defaultState, defaultState, defaultState, defaultState, defaultState
};

int movement[segSize][noOfMovements] = {
//Up Down Left Right 
  {0, 6, 5, 1},  // pinA
  {0, 6, 5, 1},  // pinB
  {6, 3, 4, 7},  // pinC
  {6, 3, 4, 2},  // pinD
  {6, 3, 4, 2},  // pinE
  {0, 6, 5, 1},  // pinF
  {0, 3, 6, 6},  // pinG
  {7, 7, 2, 7}   // pinDP
};    

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode == true) {
    state = !state;
  }
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  checkButtonPressed();
  joystickMovement();

  if (currentState == 1) {
    displayMovement(currentSegment, lastSegment);
  } 
}

void checkButtonPressed() {
  reading = digitalRead(pinSW);
 
  if (reading != lastSwState){
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
            resetDisplay();
          } else if (pressDuration < shortPressTime) {
            if (currentState == 1) {
              currentState = 2;
            } else if(currentState == 2) {
              currentState = 1;
            }
          }
      }
    }
  }
  lastSwState = reading;
}

void joystickMovement() {
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && joyMoved == false) { // UP OR LIGHT
    if (currentState == 1) {
      currentSegment = movement[lastSegment][0]; 
    } else if (currentState == 2) {
      digitalWrite(segments[currentSegment], HIGH);
      segmentStates[currentSegment] = HIGH;
    }
    joyMoved = true;
  }

  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && joyMoved == false) {  // DOWN OR DARK
    if (currentState == 1) {
      currentSegment = movement[lastSegment][1]; 
    } else if (currentState == 2) {
      digitalWrite(segments[currentSegment], LOW); 
      segmentStates[currentSegment] = LOW;
    }
    joyMoved = true;
  }

  if (currentState == 1) {  
    if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && joyMoved == false) { // LEFT
      currentSegment = movement[lastSegment][2]; 
      joyMoved = true;
    }
    if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && joyMoved == false) { // RIGHT
      currentSegment = movement[lastSegment][3]; 
      joyMoved = true;
    }
  }

  if (xValue > minThreshold && xValue < maxThreshold && yValue > minThreshold && yValue < maxThreshold) {
    lastSegment = currentSegment;
    joyMoved = false;
  }
}

void displayMovement(int currentSegment, int lastSegment) {
  digitalWrite(segments[lastSegment], segmentStates[lastSegment]);

  if (millis() - lastBlink > blinkTime) {
    blink = !blink;
    digitalWrite(segments[currentSegment], blink);
    lastBlink = millis();
  }
  digitalWrite(segments[currentSegment], blink);
}

void resetDisplay() {
  for (int i = 0; i < segSize; i++) {
    digitalWrite(segments[i], LOW);
    segmentStates[i] = LOW;
  }
  currentSegment = 7;
}
        
