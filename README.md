# Introduction To Robotics (2022 - 2023)

Introduction to Robotics laboratory homeworks, 3rd year, 1st semester at the Faculty of Matematics and Computer Science, University of Bucharest. 

This repository contains the weekly homeworks that include requirements, code, implementation details and images.

### Homework #0: Setup :heavy_check_mark:
---
**Install Arduino IDE** 

<details><summary>Picture of the setup</summary>
  
![arduino_dovada](https://user-images.githubusercontent.com/86727047/196508227-7b9e3b06-2112-4c6b-9854-d9cc0251f7e2.png)

</details>

**Create this public repository** 


### Homework #1: RGB led :bulb:
---
**Task Requirements**: I used a separat potentiometer in controlling each of the color of the RGB led (Red, Green and Blue).  The control is done with digital electronics (aka i read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connectedto the led).

 <details><summary>Pictures of the setup</summary>

![Image 1](https://user-images.githubusercontent.com/86727047/197394418-cf18ab85-059f-4382-b20b-33862efd86c1.jpeg)

![Image 2](https://user-images.githubusercontent.com/86727047/197394420-4530214e-3be1-496f-8828-5c7365cf365f.jpeg)

![Image 3](https://user-images.githubusercontent.com/86727047/197394422-fb8d6c88-c8db-495f-85a5-aa05c544ec3d.jpeg)
  
</details>

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=hZ9irYRZ2V8)


### Homework #2: Traffic lights for a crosswalk  :vertical_traffic_light: :walking_woman:
---
**Task Requirements**: I used 2 leds to represent the traffic lights for people and 3 leds to represent the traffic lights for people. It's a traffic light system just like in real life: green light for cars means that is red light for people, yellow light for cars means still red for people, red light for cars means that is green light for people and a buzzer starts beeping. When the red light for cars is coming to an end, the beep sounds faster.

 <details><summary>Picture of the setup</summary>

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
:grey_exclamation: Using the joystick, i can move between segments and "draw" on the display. The movement is natural meaning that from the current position i can move only to neighbords. 
:grey_exclamation: On the default state, the current segment is blinking. Using the joystick i move to other segment (just neighbors). 
:grey_exclamation: If i press short the button, the current segment stop blinking. Moving the joystick up and down i modify the state of the segmnet (UP = ON and DOWN = OFF). Clinking again short the joystick, the segment state is saved and exit to state 1 (default state). Clinking long doesn't trigger anything.
:grey_exclamation: If i press long the button from state 1, it resets the entire display by turning all the segments OFF and moving the current position to the decimal point.

<details><summary>Picture of the setup</summary>

![Image 1](https://user-images.githubusercontent.com/86727047/201165885-803338be-4dea-424f-987d-18898eb74f41.jpg)

![Image 2](https://user-images.githubusercontent.com/86727047/201165522-3fd1a30c-d08f-4acc-ada5-c0c0cdd25d9b.jpg)

![Image 3](https://user-images.githubusercontent.com/86727047/201165856-08c59a6b-9c90-422b-bac6-ec947bea1804.jpg)

</details>  

[**Link to video showcasing functionality**](https://www.youtube.com/watch?v=QNXynbl0U4Y)
