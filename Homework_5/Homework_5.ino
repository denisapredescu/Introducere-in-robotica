// values position in EEPROM:
// position 0: lcdbrightness (value between 0 and 255)
// position 1: matrixbrightness (value between 0 and 15)
// position 2: sound (0 or 1)
// position 9: numberOfHighscores (max 5)
// then leaderboard:
//      positions 10 - 14  name 1
//      positions 16 - 18  score 1
//      positions 20 - 24  nume 2
//      positions 26 - 28  score 2
//      ...
// the score can be a integer greater than 0 and smaller than 65536


#include <LiquidCrystal.h>  // library for display
#include "LedControl.h"     // library for matrix
#include <EEPROM.h>         // library for eeprom: read/write data
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessControlPin = 6;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// joystick setup
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;
const int buzzer = A2;
byte swState = HIGH;
byte lastSwState = HIGH;
int xValue = 0;
int yValue = 0;
bool xJoystickState = false;
bool yJoystickState = false;
const int minThreshold = 400;
const int maxThreshold = 600;

byte reading = 1;
unsigned long lastDebounceTime = 0;
const unsigned int debounceDelay = 50;
unsigned long startGreetingTime = 0;
const unsigned int greetingDuration = 4000;
const unsigned int endingMessageDuration = 5000;
unsigned long startEndingMessage = 0;

byte indexText = 1;
const String aboutSection[] = {
  "Game: SNAKE",
  "Author: Denisa Predescu",
  "GitHub: https://github.com/denisapredescu",
  "Project for Introduction to Robotics, Unibuc",
  "Press to exit"
};

// menu state: represent the menu/spot that is displayed
// 0. main menu
// 1. start game
// 2. view leaderboard
// 3. settings (list of different settings)
// 4. about (info about the game and the creator of it)
// 5. how to play (info about the game)
byte menuState;

// main menu => se refera unde sunt in lista cu cerinte a meniului principal
// the place where the symbol ">" is displays  (e.g. ">Start game" means mainMenuState = 0)
byte mainMenuState;

// like the menu above, we need to know where we are in a menu/list of texts
byte infoMenuState;
byte settingsMenuState;
byte howToPlayMenuState;
byte leaderboardMenuState;

bool nameAddedFromSettings = false;
bool nameAddedAfterPlaying;
String playerName = "AAAAA";  // global name, created/updated in settings menu
String name;                  // is necessary for creating a temporary name for a new game played
                              // by a person that not inserted a name in settings
byte yJoystickPosition = 0;   // for changing letters from player name
byte difficulty = 0;
bool sound;
bool gameOverSoundWasPlayed = false;

// values for brightness
byte lcdBrightness;
byte matrixBrightness;

// brightnessItem has 2 values:
// 0 if we are on the lcd brightness element
// 1 if we are on the lcd brightness element
byte brightnessItem;

byte numberOfHighscores;
String leaderboardNames[5] = {};
int highscores[5] = {};
bool checkHighscore = false;
byte leaderboardPosition;

// game setup
bool playing;
byte snakeColumn;
byte snakeRow;
byte foodColumn;
byte foodRow;
long score;
byte lives;

// blinking interval
const int blinkTimeOn = 400;
const int blinkTimeOff = 800;
unsigned long lastBlink = 0;

byte arrowUp[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b00000
};

byte arrowDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};

byte currentLetterArrow[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte saveArrow[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b00001,
  0b00010,
  0b00100,
  0b00000,
  0b00000
};

byte speaker[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte soundOff[8] = {
  0b00000,
  0b00000,
  0b10010,
  0b01100,
  0b01100,
  0b10010,
  0b00000,
  0b00000
};

byte soundOn[8] = {
  0b11000,
  0b00100,
  0b10010,
  0b01010,
  0b01010,
  0b10010,
  0b00100,
  0b11000
};

byte brightnessBar[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte brightnessPlaceholder[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte tailOfSnake[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b01111,
  0b00111,
  0b00001,
  0b00000,
  0b00000
};

byte middleOfSnake[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b11111,
  0b11111,
  0b10011,
  0b00000,
  0b00000
};

byte headOfSnake[8] = {
  0b00000,
  0b00000,
  0b01100,
  0b11110,
  0b11111,
  0b11110,
  0b01100,
  0b00000
};

byte displayImageOnMatrix;
byte lastDisplayImageOnMatrix;
const uint8_t matrixImages[][8] = {
  { 0b00000000,
    0b01001010,
    0b01001000,
    0b01001010,
    0b01111010,
    0b01001010,
    0b01001010,
    0b01001010 },
  { 0b00000000,
    0b01100110,
    0b10011001,
    0b10000001,
    0b10000001,
    0b01000010,
    0b00100100,
    0b00011000 },
  { 0b00000000,
    0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00000000 }
};

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);

  // setupEEPROM(); // run only once to insert default values in eeprom
  readDataFromEEPROM();

  // set the default value for lcd and lc
  pinMode(lcdBrightnessControlPin, OUTPUT);
  analogWrite(lcdBrightnessControlPin, lcdBrightness);
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)

  startGreetingTime = millis();
  menuState = 0;
  mainMenuState = 0;

  lcd.createChar(6, arrowUp);
  lcd.createChar(7, arrowDown);

  changeMatrixImage(0);  // welcome image for matrix
  displayImageOnMatrix = 0;
  lastDisplayImageOnMatrix = 0;

  // image of snake for welcome display
  lcd.createChar(0, tailOfSnake);
  lcd.createChar(1, middleOfSnake);
  lcd.createChar(2, headOfSnake);
}

void loop() {
  if (displayImageOnMatrix != lastDisplayImageOnMatrix) {
    changeMatrixImage(displayImageOnMatrix);
    lastDisplayImageOnMatrix = displayImageOnMatrix;
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  reading = digitalRead(pinSW);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if (millis() - startGreetingTime < greetingDuration) {  // greeting message/image displayed for a few moments
    lcd.setCursor(5, 0);
    lcd.print("SNAKE");
    lcd.setCursor(3, 1);
    lcd.write((byte)0);
    for (int i = 0; i < 7; i++) {
      lcd.setCursor(i + 4, 1);
      lcd.write((byte)1);
    }
    lcd.setCursor(11, 1);
    lcd.write((byte)2);
  } else {
    if (millis() - lastDebounceTime > debounceDelay) {
      if (reading != swState) {
        swState = reading;
      }
    }
    lastSwState = reading;
    // check what menu is displayed
    if (menuState == 0) {  // main menu
      displayImageOnMatrix = 1;
      delay(30);
      switch (mainMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Start Game");
          lcd.setCursor(1, 1);
          lcd.print("Leaderboard");

          lcd.setCursor(15, 1);  // down arrow
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {  // button pressed => game starts
            playing = true;
            snakeColumn = random(matrixSize);
            snakeRow = random(matrixSize);
            foodColumn = random(matrixSize);
            foodRow = random(matrixSize);
            score = 0;
            lives = 3;
            gameOverSoundWasPlayed = false;
            yJoystickPosition = 0;
            lcd.createChar(0, heart);
            menuState = 1;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 1, mainMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Start Game");
          lcd.setCursor(0, 1);
          lcd.print(">Leaderboard");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            leaderboardMenuState = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Leaderboard");
          lcd.setCursor(0, 1);
          lcd.print(">Settings");

          lcd.setCursor(15, 0);  // up arrow
          lcd.write((byte)6);
          lcd.setCursor(15, 1);  // down arroe
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 4, 3);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Leaderboard");
          lcd.setCursor(1, 1);
          lcd.print("Settings");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            leaderboardMenuState = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Settings");
          lcd.setCursor(0, 1);
          lcd.print(">About");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            infoMenuState = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 6, 5);
          break;

        case 5:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Settings");
          lcd.setCursor(1, 1);
          lcd.print("About");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 4, 3);
          break;

        case 6:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("About");
          lcd.setCursor(0, 1);
          lcd.print(">How to play");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            howToPlayMenuState = 0;
            menuState = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, mainMenuState, 7);
          break;

        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">About");
          lcd.setCursor(1, 1);
          lcd.print("How to play");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            infoMenuState = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuState = setMenuState(mainMenuState, 6, 5);
          break;
      }
    } else if (menuState == 1) {  // start game
      if (playing) {
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, false);  // turns off on LED
          }
        }
        foodLogic();
        displayInGameStats();  // display relevant info: name, difficulty, lives, score

        lc.setLed(0, snakeRow, snakeColumn, true);  // snake position
        snakeMovement();

        if (score == 10) {  // game over
          playing = false;
          displayImageOnMatrix = 2;
          if (sound && !gameOverSoundWasPlayed) {  // game over sound
            gameOverSoundWasPlayed = true;
            gameOverSound();
          }
          resetMatrix();
          startEndingMessage = millis();
        }
      } else {
        lcd.createChar(0, currentLetterArrow);  // if the name is not set, the characters are needed
        lcd.createChar(1, saveArrow);

        gameEnded();
        if (checkHighscore) {  // display messages for the player status
          lcd.clear();
          if (leaderboardPosition < 5) {
            displayImageOnMatrix = 1;
            lcd.setCursor(2, 0);
            lcd.print("You made it!");
            if (leaderboardPosition == 0) {
              lcd.setCursor(1, 1);
              lcd.print("Highest score!");
            } else {
              lcd.setCursor(3, 1);
              lcd.print("Top 5! :)");
            }
          } else {
            lcd.setCursor(1, 0);
            lcd.print("Your score is");
            lcd.setCursor(0, 1);
            lcd.print("not good enought");
          }
          delay(50);

          if (swState == LOW) {
            checkHighscore = false;
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
        }
      }
    } else if (menuState == 2) {  // view leaderboard; first 5 hightscores
      if (numberOfHighscores == 0) {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Leaderboard");
        lcd.setCursor(0, 1);
        lcd.print("Nothing here...");
        delay(50);
      } else {
        switch (leaderboardMenuState) {
          case 0:
            lcd.clear();
            lcd.setCursor(3, 0);
            lcd.print("Leaderboard");

            lcd.setCursor(0, 1);
            lcd.print("1." + leaderboardNames[0]);
            lcd.setCursor(11, 1);
            lcd.print(highscores[0]);

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 1, leaderboardMenuState);
            break;

          case 1:
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("1." + leaderboardNames[0]);
            lcd.setCursor(11, 0);
            lcd.print(highscores[0]);

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[1]);
            } else {
              lcd.print("2.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 2, 0);
            break;

          case 2:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[1]);
            } else {
              lcd.print("2.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[2]);
            } else {
              lcd.print("3.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 3, 1);
            break;

          case 3:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[2]);
            } else {
              lcd.print("3.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[3]);
            } else {
              lcd.print("4.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 4, 2);
            break;

          case 4:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[3]);
            } else {
              lcd.print("4.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[4]);
            } else {
              lcd.print("5.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 5, 3);
            break;

          case 5:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[4]);
            } else {
              lcd.print("5.");
            }

            lcd.setCursor(0, 1);
            lcd.print("Press to exit");

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, leaderboardMenuState, 4);
            break;
        }
      }
      if (swState == LOW) {  // in this case, whenever the player wnts to exit leaderborad list, he can press the button and go back
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }
    } else if (menuState == 3) {  // clicked on setting, show settings menu
      switch (settingsMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Enter name");
          lcd.setCursor(1, 1);
          lcd.print("Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            yJoystickPosition = 0;
            settingsMenuState = 8;  // go to enter name section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 1, settingsMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Enter name");
          lcd.setCursor(0, 1);
          lcd.print(">Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 9;  // go to set difficulty section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Difficulty");
          lcd.setCursor(0, 1);
          lcd.print(">Brightness");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuState = 11;  // go to set brightness for lcd and matrix section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 4, 3);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Difficulty");
          lcd.setCursor(1, 1);
          lcd.print("Brightness");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 9;  // go to set difficulty section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Brightness");
          lcd.setCursor(0, 1);
          lcd.print(">Sound");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 10;  // go to set sound section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 6, 5);
          break;

        case 5:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Brightness");
          lcd.setCursor(1, 1);
          lcd.print("Sound");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuState = 11;  // go to set brightness for lcd and matrix section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 4, 3);
          break;

        case 6:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Sound");
          lcd.setCursor(0, 1);
          lcd.print(">Press to exit");  // exit "button"

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            menuState = 0;  // go back to main menu
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, settingsMenuState, 7);
          break;

        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Sound");
          lcd.setCursor(1, 1);
          lcd.print("Press to exit");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 10;  // go to set sound section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 6, 5);
          break;

        case 8:  // edit name (be pressing the joystick from case 0 - enter name)
          lcd.createChar(0, currentLetterArrow);
          lcd.createChar(1, saveArrow);

          nameAddedFromSettings = true;  // if we are here means that the player choose to add his name
          nameAddedAfterPlaying = false;

          playerName = setName(playerName);
          if (swState == LOW && yJoystickPosition == 5) {  // yJoystickPosition == 5 if the player wants to save the current his name
            settingsMenuState = 0;                         // go back to settings menu, display 0
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 9:  // set difficulty
          lcd.createChar(0, currentLetterArrow);

          setDifficulty();
          if (swState == LOW) {
            settingsMenuState = 1;  // go back to settings menu
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 10:  // set sound on/off
          lcd.createChar(0, speaker);
          lcd.createChar(1, soundOn);
          lcd.createChar(2, soundOff);
          lcd.createChar(3, saveArrow);

          setSound();
          if (swState == LOW) {
            EEPROM.put(2, sound);  // save the sound setting
            settingsMenuState = 7;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 11:  // brightness for lcd and matrix settings menu
          lcd.createChar(0, brightnessBar);
          lcd.createChar(1, brightnessPlaceholder);

          setBrightness();
          if (swState == LOW) {  // go back to settings menu to "Brightness" line and save the edited info
            EEPROM.put(0, lcdBrightness);
            EEPROM.put(1, matrixBrightness);
            settingsMenuState = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;
      }
    } else if (menuState == 4) {  // about section
      switch (infoMenuState) {
        case 0:
          lcd.clear();
          displayText(aboutSection[1], 1, 1, false);
          displayText(aboutSection[0], 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 1, infoMenuState);
          break;

        case 1:
          lcd.clear();
          displayText(aboutSection[0], 1, 0, false);
          displayText(aboutSection[1], 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          displayText(aboutSection[1], 1, 0, false);
          displayText(aboutSection[2], 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 4, 3);
          break;

        case 3:
          lcd.clear();
          displayText(aboutSection[2], 1, 1, false);
          displayText(aboutSection[1], 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          displayText(aboutSection[2], 1, 0, false);
          displayText(aboutSection[3], 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 6, 5);
          break;

        case 5:
          lcd.clear();
          displayText(aboutSection[3], 1, 1, false);
          displayText(aboutSection[2], 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 4, 3);
          break;

        case 6:
          lcd.clear();
          displayText(aboutSection[3], 1, 0, false);
          displayText(aboutSection[4], 1, 1, true);
          delay(50);

          if (swState == LOW) {
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
          infoMenuState = setMenuState(infoMenuState, infoMenuState, 7);
          break;

        case 7:
          lcd.clear();
          displayText(aboutSection[4], 1, 1, false);
          displayText(aboutSection[3], 1, 0, true);
          delay(50);
          infoMenuState = setMenuState(infoMenuState, 6, 5);
          break;
      }
    } else if (menuState == 5) {  // how to play section
      switch (howToPlayMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("The snake wants");
          lcd.setCursor(0, 1);
          lcd.print("to eat the");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 1, howToPlayMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("blinking food.");
          lcd.setCursor(0, 1);
          lcd.print("Every food means");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("1 pt. The game");
          lcd.setCursor(0, 1);
          lcd.print("ends when the ");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 3, 1);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("score is 10.");
          lcd.setCursor(0, 1);
          lcd.print("Press to exit");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }

          howToPlayMenuState = setMenuState(howToPlayMenuState, howToPlayMenuState, 2);
          break;
      }
    }
  }
}

// function that moves the display up, down or stay still
int setMenuState(int stay, int goDown, int goUp) {
  if (xValue < minThreshold && !xJoystickState) {
    indexText = 1;  // for scrollable text that can be found in list (e.g. about section)
    xJoystickState = true;
    return goDown;
  }
  if (xValue > maxThreshold && !xJoystickState) {
    indexText = 1;
    xJoystickState = true;
    return goUp;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
    return stay;
  }
}

String setName(String name) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(name);
  lcd.setCursor(12, 0);
  lcd.print("save");

  if (yJoystickPosition < 5) {
    lcd.setCursor(1 + yJoystickPosition, 1);
    lcd.write((byte)0);  // arrow below letter
  } else {
    lcd.setCursor(11, 0);
    lcd.write((byte)1);  // arrow pointing to "save"
  }
  delay(50);

  movingBetweenPositions();  // joystick movement
  if (yJoystickPosition < 5) {
    name = movingBetweenLetters(name);  // A - Z
  }
  return name;
}

String movingBetweenLetters(String name) {
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {  //DOWN
    if (name[yJoystickPosition] != 'A') {
      name[yJoystickPosition] = (char)(name[yJoystickPosition] - 1);
    } else {
      name[yJoystickPosition] = 'Z';
    }
    xJoystickState = true;
  }
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {  //UP
    if (name[yJoystickPosition] != 'Z') {
      name[yJoystickPosition] = (char)(name[yJoystickPosition] + 1);
    } else {
      name[yJoystickPosition] = 'A';
    }
    xJoystickState = true;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
  return name;
}

void movingBetweenPositions() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition > 0 ? yJoystickPosition - 1 : 0;
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition < 5 ? yJoystickPosition + 1 : 5;
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("low medium hight");

  if (difficulty == 0) {
    lcd.setCursor(1, 1);
  } else if (difficulty == 1) {
    lcd.setCursor(6, 1);
  } else if (difficulty == 2) {
    lcd.setCursor(13, 1);
  }
  lcd.write((byte)0);
  delay(50);

  movingSelectedDifficulty();
}

void movingSelectedDifficulty() {  // moving left right
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = difficulty - 1;
    Serial.println(difficulty);
    if (difficulty == 255) {
      difficulty = 2;
    }
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = (difficulty + 1) % 3;
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setSound() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write((byte)0);  // speaker
  lcd.write((byte)1);  // on

  lcd.setCursor(4, 1);
  lcd.write((byte)0);
  lcd.write((byte)2);  // off

  if (sound) {
    lcd.setCursor(2, 0);
    lcd.write((byte)3);  // arrow to save
  } else {
    lcd.setCursor(2, 1);
    lcd.write((byte)3);  // arrow to save
  }
  delay(50);

  movingOnOff();
}

void movingOnOff() {
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    sound = false;
  }
  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    sound = true;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setBrightness() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brightness for:");

  lcd.setCursor(8, 1);
  lcd.print("<");
  lcd.setCursor(14, 1);
  lcd.print(">");

  lcd.setCursor(1, 1);
  if (brightnessItem == 0) {
    lcd.print("LCD");
    analogWrite(lcdBrightnessControlPin, lcdBrightness);
  } else {
    lcd.print("Matrix");
    lc.setIntensity(0, matrixBrightness);
  }
  setBrightnessDisplay();
  delay(50);
  changingValueOfBrightness();
  movingBetweenBrightnessElements();
}

// LCD brightness take values between 0 and 255; the player can choose from 6 values: 0, 51, 102, 153, 204, 255
// Matrix brightness take values between 0 and 15; the player can choose from 6 values: 0, 3, 6, 9, 12, 15
void setBrightnessDisplay() {
  byte brightnessLevel;

  if (brightnessItem == 0) {
    brightnessLevel = lcdBrightness / 51;
  } else {
    brightnessLevel = matrixBrightness / 3;
  }
  for (int i = 0; i < brightnessLevel; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.write((byte)0);
  }
  for (int i = brightnessLevel; i < 5; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.write((byte)1);
  }
}

// we move with 51 step for matrix brightness and 3 step for lcd brightness
void changingValueOfBrightness() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to left
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 0) {
        lcdBrightness -= 51;
      }
    } else {
      if (matrixBrightness != 0) {
        matrixBrightness -= 3;
      }
    }
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to right
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 255) {
        lcdBrightness += 51;
      }
    } else {
      if (matrixBrightness != 15) {
        matrixBrightness += 3;
      }
    }
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void movingBetweenBrightnessElements() {  // move between lcd brightness setup and matrix brightness setup
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    if (brightnessItem == 0) {
      brightnessItem = 1;
    }
  }
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    if (brightnessItem == 1) {
      brightnessItem = 0;
    }
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void displayText(String text, byte column, byte row, bool scrollable) {
  if (!scrollable) {
    lcd.setCursor(column, row);
    lcd.print(text.substring(0, 15));
  } else {  // if a text is scrollable, the symbol ">" is on it
    lcd.setCursor(column - 1, row);
    lcd.print(">");
    lcd.setCursor(column, row);
    if (text.length() <= 15) {
      lcd.print(text);
    } else {
      String newText = text + "  ";
      if (indexText <= newText.length()) {
        scrollText(indexText, newText);
        indexText++;
      } else {
        indexText = 1;
      }
    }
  }
}

void scrollText(int startLetter, String message) {  // logic for scrolling a text
  if (message.length() - startLetter > 15) {
    lcd.print(message.substring(startLetter, startLetter + 15));
  } else {
    lcd.print(message.substring(startLetter, message.length()) + message.substring(0, 15 - message.length() + startLetter));
  }
  delay(250);
}

// run only once to insert first data in EEPROM
void setupEEPROM() {
  lcdBrightness = 102;
  matrixBrightness = 15;
  sound = true;
  numberOfHighscores = 0;

  EEPROM.put(0, lcdBrightness);
  EEPROM.put(1, matrixBrightness);
  EEPROM.put(2, sound);
  EEPROM.put(19, numberOfHighscores);
}

// read all data from EEPROM
void readDataFromEEPROM() {
  lcdBrightness = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  sound = EEPROM.read(2);
  numberOfHighscores = EEPROM.read(19);

  for (int i = 1; i <= numberOfHighscores; i++) {
    String player = "";
    long playerScore = 0;

    for (int j = 0; j < 5; j++) {
      player += (char)EEPROM.read(i * 10 + j);
    }

    byte firstByte = EEPROM.read(i * 10 + 6);
    byte secondByte = EEPROM.read(i * 10 + 7);
    playerScore = (firstByte * 256) + secondByte;

    leaderboardNames[i - 1] = player;
    highscores[i - 1] = playerScore;
  }
}

void foodLogic() {
  if (millis() - lastBlink > blinkTimeOn) {
    lc.setLed(0, foodRow, foodColumn, true);
  }
  if (millis() - lastBlink > blinkTimeOff) {
    lc.setLed(0, foodRow, foodColumn, false);
    lastBlink = millis();
  }
  if (snakeColumn == foodColumn && snakeRow == foodRow) {
    foodColumn = random(matrixSize);
    foodRow = random(matrixSize);
    score += 1;
  }
}

void snakeMovement() {
  if (xValue > maxThreshold && !xJoystickState) {  // go up
    xJoystickState = true;
    snakeRow = snakeRow - 1;
    if (snakeRow == 255) {
      snakeRow = matrixSize - 1;
    }
  }
  if (xValue < minThreshold && !xJoystickState) {  // go down
    xJoystickState = true;
    snakeRow = snakeRow + 1;
    if (snakeRow == matrixSize) {
      snakeRow = 0;
    }
  }
  if (yValue < minThreshold && !yJoystickState) {  // go left
    yJoystickState = true;
    snakeColumn = snakeColumn - 1;
    if (snakeColumn == 255) {
      snakeColumn = matrixSize - 1;
    }
  }
  if (yValue > maxThreshold && !yJoystickState) {  // go right
    yJoystickState = true;
    snakeColumn = snakeColumn + 1;
    if (snakeColumn == matrixSize) {
      snakeColumn = 0;
    }
  }
  if (xValue > maxThreshold && yValue < minThreshold && !xJoystickState && !yJoystickState) {  // go up and left
    xJoystickState = true;
    yJoystickState = true;
    snakeRow = snakeRow - 1;
    if (snakeRow == 255) {
      snakeRow = matrixSize - 1;
    }
    snakeColumn = snakeColumn - 1;
    if (snakeColumn == 255) {
      snakeColumn = matrixSize - 1;
    }
  }
  if (xValue > maxThreshold && yValue > maxThreshold && !xJoystickState && !yJoystickState) {  // go up and right
    xJoystickState = true;
    yJoystickState = true;
    snakeRow = snakeRow - 1;
    if (snakeRow == 255) {
      snakeRow = matrixSize - 1;
    }
    snakeColumn = snakeColumn + 1;
    if (snakeColumn == matrixSize) {
      snakeColumn = 0;
    }
  }
  if (xValue < minThreshold && !xJoystickState && yValue < minThreshold && !yJoystickState) {  // go down and left
    xJoystickState = true;
    yJoystickState = true;
    snakeRow = snakeRow + 1;
    if (snakeRow == matrixSize) {
      snakeRow = 0;
    }
    snakeColumn = snakeColumn - 1;
    if (snakeColumn == 255) {
      snakeColumn = matrixSize - 1;
    }
  }
  if (xValue < minThreshold && !xJoystickState && yValue > maxThreshold && !yJoystickState) {  // go down and right
    xJoystickState = true;
    yJoystickState = true;
    snakeRow = snakeRow + 1;
    if (snakeRow == matrixSize) {
      snakeRow = 0;
    }
    snakeColumn = snakeColumn + 1;
    if (snakeColumn == matrixSize) {
      snakeColumn = 0;
    }
  }
  if (xValue < maxThreshold && xValue > minThreshold && yValue < maxThreshold && yValue > minThreshold) {
    xJoystickState = false;
    yJoystickState = false;
  }
}

// playr   Dif:Medium
// iii          xxxxx

// Display that presents the game status:
//     lives
//     scores
//     difficulty
//     player name  => if the name is set from settings menu
//                  => otherwise is shown a default name "PLAYR"
void displayInGameStats() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (nameAddedFromSettings) {  // the player has set a name for himself in settings menu
    lcd.print(playerName);
  } else {
    lcd.print("PLAYR");
    nameAddedAfterPlaying = true;
  }

  lcd.setCursor(6, 0);
  lcd.print("Dif:");

  lcd.setCursor(10, 0);
  if (difficulty == 0) {
    lcd.print(" LOW ");
  } else if (difficulty == 1) {
    lcd.print("MEDIUM");
  } else if (difficulty == 2) {
    lcd.print(" HIGH ");
  }

  for (int i = 0; i < lives; i++) {
    lcd.setCursor(i, 1);
    lcd.write((byte)0);
  }

  lcd.setCursor(6, 1);
  lcd.print("Score:");
  lcd.setCursor(12, 1);
  lcd.print(score);
}

void gameEnded() {
  lcd.clear();

  if (millis() - startEndingMessage < endingMessageDuration) {  // screen 1 is seen for a few seconds
    lcd.setCursor(3, 0);
    if (millis() - lastBlink > blinkTimeOn) {
      lcd.print("GAME OVER");
    }
    if (millis() - lastBlink > blinkTimeOff) {
      lcd.clear();
      lastBlink = millis();
    }

    lcd.setCursor(3, 1);
    lcd.print("Score:" + (String)score);
    delay(50);

    name = playerName;  // "name" will be the name entered in settings or "AAAAA" (default value)
                        // if case of default, the player will have to set a "temporary" name

  } else {  // screen 2
    if (!checkHighscore) {
      leaderboardPosition = 0;

      while (leaderboardPosition < numberOfHighscores && score <= highscores[leaderboardPosition]) {
        leaderboardPosition++;
      }

      if (leaderboardPosition < 5) {
        if (nameAddedAfterPlaying) {  // if player not inserted a name already, he has to do it now
          name = setName(name);

          if (swState == LOW && yJoystickPosition == 5) {  // the player sets his name and now he wants to exit
            nameAddedAfterPlaying = false;
            swState = HIGH;
            lastSwState = HIGH;
          }
        } else {  // the name is set, it can be saved to leaderboard
          for (int i = numberOfHighscores - 1; i > leaderboardPosition; i--) {
            highscores[i] = highscores[i - 1];
            leaderboardNames[i] = leaderboardNames[i - 1];
          }
          highscores[leaderboardPosition] = score;
          leaderboardNames[leaderboardPosition] = name;
          if (numberOfHighscores < 5) {
            numberOfHighscores = numberOfHighscores + 1;
          }
          writeLeaderboardToEEPROM();
          checkHighscore = true;
        }
      } else {
        checkHighscore = true;
      }
    }
  }
}

// save/update leaderboard to EEPROM from leaderboardNames and highscores vectors
void writeLeaderboardToEEPROM() {
  EEPROM.put(19, numberOfHighscores);

  for (int i = 1; i <= numberOfHighscores; i++) {
    for (int j = 0; j < 5; j++) {
      EEPROM.put(i * 10 + j, leaderboardNames[i - 1][j]);
      EEPROM.put(i * 10 + 7, 0);
    }

    int hix = highByte(highscores[i - 1]);
    int lox = lowByte(highscores[i - 1]);
    EEPROM.put(i * 10 + 6, hix);
    EEPROM.put(i * 10 + 7, lox);
  }
}

// function that is call when the game is over to reset the matrix
void resetMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true);  // turns on LED at col, row
      delay(20);
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turns off on LED
      delay(20);
    }
  }
}

void gameOverSound() {
  tone(buzzer, 500, 200);
  delay(100);
  tone(buzzer, 1200, 200);
  delay(100);
  tone(buzzer, 300, 200);
  delay(100);
  tone(buzzer, 1000, 200);
  delay(100);
  tone(buzzer, 400, 200);
  delay(100);
  tone(buzzer, 100, 200);
  delay(100);
}

//function that change the displayed image of the matrix
void changeMatrixImage(int index) {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turns off the matrix
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, bitRead(matrixImages[index][row], 7 - col));
    }
  }
}