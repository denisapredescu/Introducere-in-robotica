# Introduction To Robotics (2022 - 2023)

Introduction to Robotics laboratory homeworks, 3rd year, 1st semester at the Faculty of Matematics and Computer Science, University of Bucharest. 

This repository contains the weekly homeworks that include requirements, code, implementation details and images.

### Homework #0: Setup :heavy_check_mark:
---
**Install Arduino IDE** 

<details><summary><b>Picture of the setup</b></summary>
  
![arduino_dovada](https://user-images.githubusercontent.com/86727047/196508227-7b9e3b06-2112-4c6b-9854-d9cc0251f7e2.png)

</details>

**Create this public repository** 


### Homework #1: RGB led :bulb:
---
**Task Requirements**: I used a separat potentiometer in controlling each of the color of the RGB led (Red, Green and Blue).  The control is done with digital electronics (aka i read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connectedto the led).

 <details><summary><b>Pictures of the setup</b></summary>

![Image 1](https://user-images.githubusercontent.com/86727047/197394418-cf18ab85-059f-4382-b20b-33862efd86c1.jpeg)

![Image 2](https://user-images.githubusercontent.com/86727047/197394420-4530214e-3be1-496f-8828-5c7365cf365f.jpeg)

![Image 3](https://user-images.githubusercontent.com/86727047/197394422-fb8d6c88-c8db-495f-85a5-aa05c544ec3d.jpeg)
  
</details>

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=hZ9irYRZ2V8)


### Homework #2: Traffic lights for a crosswalk  :vertical_traffic_light: :walking_woman:
---
**Task Requirements**: I used 2 leds to represent the traffic lights for people and 3 leds to represent the traffic lights for people. It's a traffic light system just like in real life: green light for cars means that is red light for people, yellow light for cars means still red for people, red light for cars means that is green light for people and a buzzer starts beeping. When the red light for cars is coming to an end, the beep sounds faster.

 <details><summary><b>Picture of the setup </b></summary>

![Image 1 - homework #2](https://user-images.githubusercontent.com/86727047/198901921-0ccbec14-cc9d-4096-bfeb-3ef67727b48d.jpg)

![Image 2 - homework #2](https://user-images.githubusercontent.com/86727047/198901962-27a5a459-eb9a-46f8-b34f-b7c88d17d208.jpg)

</details>

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=JYlwsl6Z6BY)


### Homework #3: Joystick controlling the position of the segment and ”draw” on the display :joystick:
---
**Components**:
- 7-segment display
- joystick
- 2 resistors
- wires

**Task Requirements**: 

:grey_exclamation: Using the joystick, i can move between segments and "draw" on the display. The movement is natural meaning that from the current position i can move only to neighbors. 

:grey_exclamation: On the default state, the current segment is blinking. Using the joystick i move to other segment (just neighbors). 

:grey_exclamation: If i press short the button, the current segment stop blinking. Moving the joystick up and down i modify the state of the segmnet (UP = ON and DOWN = OFF). Clinking again short the joystick, the segment state is saved and exit to state 1 (default state). Clinking long doesn't trigger anything.

:grey_exclamation: If i press long the button from state 1, it resets the entire display by turning all the segments OFF and moving the current position to the decimal point.

<details><summary><b> Segments movement </b></summary>
  
|CURRENT SEGMNET| UP| DOWN| LEFT| RIGHT|
| ----| ---- | ---- | ---|----|
| a| a| g| f| b|
| b| a| g| f| b|
| c| g| d| e| dp|
| d| g| d| e| c|
| e| g| d| e| c|
| f| a| g| f| b|
| g| a| d| g| g|
| dp| dp| dp| c| dp|

</details> 


<details><summary><b>Picture of the setup </b></summary>

![Image 1](https://user-images.githubusercontent.com/86727047/201165885-803338be-4dea-424f-987d-18898eb74f41.jpg)

![Image 2](https://user-images.githubusercontent.com/86727047/201165522-3fd1a30c-d08f-4acc-ada5-c0c0cdd25d9b.jpg)

![Image 3](https://user-images.githubusercontent.com/86727047/201165856-08c59a6b-9c90-422b-bac6-ec947bea1804.jpg)

</details>  

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=QNXynbl0U4Y)

### Homework #4: Joystick that moves through a 4 digit 7 segment display digits, changes the number of the digits and resets the display :joystick:
---
**Components**:
- joystick
- 4 digit 7-segment display
- 74hc595 shift register
- wires
 
**Task Requirements**: 

:pen: One important detail: the decimal points are always off except on the current digit position.

:pen: The default state is the one were we can move from one digit to another. At first, all the 4 digits are 0 and the current position is set to the first (aka rightmost) digit. A blinking decimal point shows the current digit position. By moving the joystick on Y axis, i move the current digit position left and right (i put the condition that i can not go from the leftmost digit to the rightmost digit by moving left and the same i can not go from the rightmost digit to the leftmost one by moving right). On this state, the X axis of the joystick does not do anything. 

:pen: I press short the button to lock in on the current digit. This trigger the second state. Now the decimal point from the current digit stays always on, is no longer blinking. On this second state, i can increase and decrease the display number in hex (from 0 to F; the jump from F to 0 and reverse is not possible) on the selected digit by using the joystick on the X axis (ofcourse in this state the Y axis does not do anything). By pressing again the button (long or short), i reenter the first/default state.

:pen: I press long the button to reset. When resetting, all the digits go back to 0 and the current position is set to the first digit, in the first state.

<details><summary><b>Picture of the setup </b></summary>

![WhatsApp Image 2022-11-17 at 20 06 22](https://user-images.githubusercontent.com/86727047/202525635-598cc224-4541-43ff-bf13-6590389cb3e2.jpeg)

![WhatsApp Image 2022-11-17 at 20 06 23 (1)](https://user-images.githubusercontent.com/86727047/202525650-fa4b8766-0bad-4719-a726-be814514943b.jpeg)

</details>  

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=TL89IkrLTlU)

### Homework #5: <i> Game menu </i> + snake for exemple of minigame :video_game: :snake:
---
**Components**:
- joystick (for moving and clicking)
- matrix 8x8
- MAX7219 Driver5 + 10k ohm resister
- LCD
- buzzer (for sound) + 100 ohm resistor
- 2 mini + 1 big breadboards
- wires

**Task Requirements**: 

:battery: Created a game menu that contains the basic real-world game components such as: start game, leaderboard, data about the creator of the game and how to play section. Also a settings menu where the player can ajust the game after his preferences (set a player name for him, set difficulty of the game - low, medium, high -,  turn the sound on/off, set the brightness as he please).

:battery: The leaderboard and the essential settings are saved in EEPROM for future use. 

:battery: The menu is suggestive and furthermore easy to use for a new player. :boy:

**How to play**: 

The minigame is a snake game: a snake of lenght 1 tries to eat the blinking food that is generated random on matrix. Every food increases the score with 1 point and the game ends when the snake reaches 10 points.

<details><summary><b>Picture of the setup </b></summary>

![WhatsApp Image 2022-12-09 at 23 11 14](https://user-images.githubusercontent.com/86727047/206796984-fd43c3e2-755c-4ae4-b777-bf2369468f95.jpeg)

![WhatsApp Image 2022-12-09 at 23 11 14 (1)](https://user-images.githubusercontent.com/86727047/206797000-3782eb41-0ed5-4d53-a038-dc49a52ad1c2.jpeg)

</details>  

[**Link to video showcasing functionality, version with potentiometer**](https://www.youtube.com/watch?v=bCongoafsAU&ab_channel=DenisaPredescu)
[**Link to video showcasing functionality, version with LDC contrast in settings menu**](https://www.youtube.com/watch?v=Ocb6fcoXN3g&ab_channel=DenisaPredescu)
