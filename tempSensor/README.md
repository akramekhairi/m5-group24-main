# Part I - M5Atom Temperature Sensor
---
The sensor can be run by compiling the tempSensor.ino file and uploading it to the M5Atom (with the [M5Atom library](https://github.com/m5stack/M5Atom) already installed.) Note that the device uses the internal temperature sensor of the MPU6886, so the device heating up from LEDs will have noticable effects on the temperature readings.

Initially, the device in in a "hibernate" mode where only temperature reading is being carried out. Clicking the screen button will enter changing mode, and keep the M5Atom face down will hibernate the device again. Holding the M5Atom approximately vertical (with the USB-C port facing downwards), when tilting left and right the mode number shown will change. There are 5 selectable modes, and clicking the screen button will enter the current mode shown.

**Mode 1: Show active temperature and units**
Choosing this mode will allow the M5Atom to read the current temperature and display it to the screen. The default precision is 4 decimal places.

**Mode 2: Show average temperature and units**
Choosing this mode would have the M5Atom calculating the average temperature based on the most recent 24 hours (or less) data. The default precision is also 4 units.

**Mode 3: Show color scale of temperature range and current temperature as color**
Choosing this mode, the Atom will scroll through 5 temperature marks corresponding to 5 colors, from green (lowest) to yellow (midway) to red (highest). These marks are calculated based on the available temperature data recorded up to that point. The current temperature is shown finally with a color that would visually help the user place it somewhere inbetween the 5 other temperature marks. (if the temperature exceed the bounds of the available temperature data, it will either be colored green (colder than the coldest available temperature) or red (hotter than the hottest value.)

**Mode 4: Show graph of temperature scale**
Choosing this mode, the M5Atom will go through the 5 marks as above, and after it will print a graph of available hourly values by using colors. The orientation is identified by a white pixel, which signify the **bottom right** of the graph, and the oldest to newest value can be read in a left-to-right, up-to-down fashion.

**Mode 5: Change units**
Choosing this mode will allow the user to press the screen button and cycle through the 3 available units: Celsius (`C`), Fahrenheit (`F`) and Kelvin (`K`). Tilting the device will exit the mode.

**Others**
The duration of the scrolling values can be changed by the symbolic constant `TXTDUR` defined at the top. The default value is 1.25 (seconds), the time it takes for one character to scroll through the screen.
