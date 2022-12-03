# M5 Stack Atom Matrix Challenge 

This repository contains the source code for three different applications of the M5 Stack Atom Matrix device. This project was completed by Group 24 (Akram Khairi, Minh Quan Nham, Shaza Elsharief).
 
## Project Description ##

### Part I - The First (Bikelight) ###
 This folder contains the code for the first part of the challenge, which involves programming the M5 Atom to function as a bikelight. The bikelight has the following modes:
 
1. OFF: All LEDs off
2. Manual Rear strobe (RED): LEDs strobe at a predetermined interval
3. Manual Front strobe (WHITE): LEDs strobe at a predetermined interval
4. Automatic Rear Mode Rear (RED): LEDs are solid during a braking event. Return to strobe when riding.
5. Automatic Front Mode Rear (White): LEDs are solid during a braking event. Return to strobe when riding

The user is able to switch between the modes by clicking the on-screen button.

### Part I - The Second (Temperature sensor) ###
This folder contains the code for the second part of the challenge, which involves programming the M5 Atom to function as a temperature sensor. The temperature sensor has the following modes:

1. Show Active temperature + Units
2. Show average of last 24 hours of temperature + Units
3. Show color scale of temperature range + current temperature as color
4. Show graph of temperature across a predefined range.
5. Change units

In normal operation, the device records the temperature but nothing is displayed on the screen of the M5 Atom. By pressing the on-screen button, the device is activated.  The user is able to change between the five modes by tilting the device left or right. If the device is turned face down, it returns to background reading mode. 

### Part II - Fidget Device  ###
This folder contains the code for the second part of the challenge, which involves programming the M5 Atom to function as a fidget device. The code creates a WiFi access point, allowing the user to access a webpage. The device monitors fidgeting and shows information both on the M5 screen and the webpage about the trends of fidgeting. The device is designed to have three modes:
1. Click Mode: In this mode, the device displays a counter which is incremented everytime the user fidgets by pressing the on-screen button. The counter is also displayed on the webpage.
2. Spin Mode: In this mode, the device records the spin time whenever the device is spun by the user and displays it on the webpage. 
3. Art Mode: To reassure the user when fidgeting, the user can draw a pixel art image by clicking the on-screen button.

This code is complemented by the design of a PCB board and a housing to allow some of the functionalities described, such as the spinning mode.

## Libraries Needed ##
The code requires the use of the "M5Atom.h" library for Part I, in addition to "WiFi.h",  "WiFiClient.h", and "WiFiAP.h" libraries for Part II (Arduino IDE). Additional .cpp and .h files required can be found in the respective folders. 

