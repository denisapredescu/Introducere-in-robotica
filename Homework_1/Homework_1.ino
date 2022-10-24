const int potRedPin = A0;
const int potGreenPin = A1;
const int potBluePin = A2;

const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

int potRedValue = 0;
int potGreenValue = 0;
int potBlueValue = 0;

const int minPotValue = 0;
const int maxPotValue = 1023;
const int minLedValue = 0;
const int maxLedvalue = 255;

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(potRedPin, INPUT);
  pinMode(potGreenPin, INPUT);
  pinMode(potBluePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  potRedValue = analogRead(potRedPin);
  potGreenValue = analogRead(potGreenPin);
  potBlueValue = analogRead(potBluePin);
  
  setColor(potRedValue, potGreenValue, potBlueValue);
}

void setColor(int potRedValue, int greenPotValue, int potBlueValue) {
  analogWrite(redPin, map(potRedValue, minPotValue, maxPotValue, minLedValue, maxLedvalue));
  analogWrite(greenPin, map(potGreenValue, minPotValue, maxPotValue, minLedValue, maxLedvalue));
  analogWrite(bluePin, map(potBlueValue, minPotValue, maxPotValue, minLedValue, maxLedvalue));
}
