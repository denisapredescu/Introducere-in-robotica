const int buttonPin = 2;
const int buzzerPin = 3;
const int redPinForCars = 4;
const int yellowPinForCars = 5;
const int greenPinForCars = 6;
const int redPinForPeople = 7;
const int greenPinForPeople = 8;

unsigned long timer = 0;
const int timerState1 = 8000;
const int timerState2 = 3000;
const int timerState3 = 8000;
const int timerState4 = 4000;

const int buzzerTone = 1000;
const int buzzerInterval = 250;
const int buzzerFasterInterval = 150;
unsigned long lastBuzzer = 0;

const int blinkTime = 250;
unsigned long lastBlink = 0;
byte blink = LOW;

int currentState = 1;
byte reading = 0;
byte buttonState = LOW;
byte buttonPushed = 0;
byte lastButtonState = LOW;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

void setup() {
  pinMode(redPinForCars,OUTPUT);
  pinMode(yellowPinForCars,OUTPUT);
  pinMode(greenPinForCars,OUTPUT);
  pinMode(redPinForPeople,OUTPUT);
  pinMode(greenPinForPeople,OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){
  checkButtonPushed();
  changeState();

  if (currentState == 1){
    state1();
  } else if (currentState == 2) {
    state2();
  } else if (currentState == 3) {
    state3();
  } else if (currentState == 4) {
    state4();
  }
}

void checkButtonPushed() {
  reading = digitalRead(buttonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW && currentState == 1) {
        if (buttonPushed == 0) {
          buttonPushed = 1;
          timer = millis();
        } 
      }
    }
  }
  lastButtonState = reading;
}

void changeState() {
    if (currentState == 1 && buttonPushed == 1 && millis() - timer > timerState1) {
    buttonPushed = 0;
    currentState = 2;
    timer = millis();
  } else if (currentState == 2 && millis() - timer > timerState2) {
    currentState = 3;
    timer = millis();
  } else if (currentState == 3 && millis() - timer > timerState3) {
    currentState = 4;
    timer = millis();
  } else if (currentState == 4 && millis() - timer > timerState4) {
    currentState = 1;
    timer = millis(); 
  }
}

void state1() {
  digitalWrite(redPinForCars, LOW);
  digitalWrite(yellowPinForCars, LOW);
  digitalWrite(greenPinForCars, HIGH);
  digitalWrite(redPinForPeople, HIGH);
  digitalWrite(greenPinForPeople, LOW);
  noTone(buzzerPin);
}

void state2() {
  digitalWrite(redPinForCars, LOW);
  digitalWrite(yellowPinForCars, HIGH);
  digitalWrite(greenPinForCars, LOW);
  digitalWrite(redPinForPeople, HIGH);
  digitalWrite(greenPinForPeople, LOW);
  noTone(buzzerPin);
}

void state3() {
  digitalWrite(redPinForCars, HIGH);
  digitalWrite(yellowPinForCars, LOW);
  digitalWrite(greenPinForCars, LOW);
  digitalWrite(redPinForPeople, LOW);
  digitalWrite(greenPinForPeople, HIGH);

  if (millis() - lastBuzzer > buzzerInterval) {
    noTone(buzzerPin);
  }
  if (millis() - lastBuzzer > 2 * buzzerInterval){
    tone(buzzerPin, buzzerTone);
    lastBuzzer = millis();
  }  
}

void state4() {
  digitalWrite(redPinForCars, HIGH);
  digitalWrite(yellowPinForCars, LOW);
  digitalWrite(greenPinForCars, LOW);
  digitalWrite(redPinForPeople, LOW);
  
  if (millis() - lastBlink > blinkTime) {
    blink = !blink;
    digitalWrite(greenPinForPeople, blink);
    lastBlink = millis();
  }
  digitalWrite(greenPinForPeople, blink);

  if (millis() - lastBuzzer > buzzerFasterInterval) {
    noTone(buzzerPin);
  }
  if (millis() - lastBuzzer > 2 * buzzerFasterInterval){
    tone(buzzerPin, buzzerTone);
    lastBuzzer = millis();
  } 
}
