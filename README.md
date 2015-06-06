#WiFi-controlled iRobot Roomba
 
Control the movement of an iRobot Roomba using any device on the same WiFi connection. Roomba is connected to an Arduino Uno with WiShield that connects to the internet. Once a telnet connection with the Arduino is established, you can control the Roomba's movements without the hassles of cables! Controls are defined accoring to the standard WASD key combination.

#Applications

* Bomb diffusal, or related applications, where human presence is not desired and you wish the robot to take the chance.
* Hotels: Drive a room service delivery cart
* Hospitals: Carry supplies to patient's room
* [with camera streaming capability] Home security robot 

With further enhancements, like the addition of camera streaming or a robotic arm on the roomba, this device can have significantly important industrial usage.


#Circuit

* Arduino Uno
* WiShield 
* iRobot Roomba with Create module

#Software

I used Arduino IDE 1.6.4 on MacOS. The Arduino code, to my knowledge, would remain the same across platforms.


#Connections

* The WiShield attaches directly on top of the Arduino Uno. 
* Make sure not to use pins 10, 11 or 12. These are already used. 
* The Roomba pinout looks like this: 
![Roomba Create Pinout](/Users/shobhit/GitHub/Roomba-WiFi/roomba_create_pinout.jpg)
* I made the following pin connections:
     Arduino pin 8  --->  Create pin 2      
     Arduino pin 9  --->  Create pin 1
     Arduino Vin    --->  Create pin 8
     Arduino GND    --->  Create pin 14
* Create Pin 8 drives the signal at 5V. This is needed. 
* To make the setup truly mobile, once the Arduino has been programmed from a computer, I
powered it using a 9V battery. 


#Instructions: 

0. Make sure Roomba is turned off. 
1. Compile this sketch and run the Serial monitor. Arduino, with
WiShield, attempts to connect to the specified WiFi network and gets 
an IP address. 
Note: At this point, Arduino and your PC can be disconnected. 
2. Telnet to your device's IP address.
3. Turn on Roomba. Press ? [return] to check availability.
4. Press r to establish Arduino's connection with Roomba.
5. Play!


#Controls
Key : Direction
  w : forward
  s : backward
  a : left
  d : right
  q : stop


#Note
This example is written for a network using WPA encryption. For
WEP or WPA, change the Wifi.begin() call accordingly.


#License
This project is released as an open source project under the MIT License. 

Copyright (C) 2015 Shobhit Garg. All rights reserved.
