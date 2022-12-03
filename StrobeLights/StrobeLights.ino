/*Part 1 of Assignment II*/

#include "M5Atom.h"
#include "TextDisplay.h"

// Choose whether description of each mode will be shown
#define SHOW_DESC 1
#define TXTDUR 1.35
bool ledstrobestate = false; //Boolean which decides whether the matrix will be off(black) or a color in order to create the strobe or blink
unsigned long lastms = 0; //Stores the last time the LEDs switched states or colors
unsigned long lastaccelms = 0;
const unsigned long strobeinterval = 100; //The strobe interval (100 ms) could be edited in order to change how often the leds blink
const unsigned long accelinterval = 25; //Accel interval could be changed in order to improve as well
float accX = 0, accY = 0, accZ = 0, avgZ1 = 0, avgZ2 = 0, acceldiff = 0;

void setup()
{
  M5.begin(true, false, true);
  M5.IMU.Init();
  Serial.begin(115200);
}

uint8_t modenumber = 0;
int avg1counter = 0;
int avg2counter = 0;

int lastmode = 0;

void loop()
{

  unsigned long currentms = millis(); //Stores the current time the program has been running for in milliseconds

  //If the button is pressed change modes
  if (M5.Btn.wasPressed())
  {
    modenumber++;
    if (SHOW_DESC) {
      switch (modenumber) {
        case 1:
          // CharacterDisplay ('1', 0.8, 87, 6, 140, true, true, 0.1);
          TextDisplay("1Rear", TXTDUR, 87, 6, 140);
          break;
        case 2:
          TextDisplay("2Front", TXTDUR, 87, 6, 140);
          break;
        case 3:
          TextDisplay("3Auto Rear", TXTDUR, 87, 6, 140);
          break;
        case 4:
          TextDisplay("4Auto Front", TXTDUR, 87, 6, 140);
          break;
      }
    } else {
      switch (modenumber) {
        case 1:
          CharacterDisplay ('1', 0.8, 87, 6, 140, true, true, 0.1);
          break;
        case 2:
          CharacterDisplay ('2', 0.8, 87, 6, 140, true, true, 0.1);
          break;
        case 3:
          CharacterDisplay ('3', 0.8, 87, 6, 140, true, true, 0.1);
          break;
        case 4:
          CharacterDisplay ('4', 0.8, 87, 6, 140, true, true, 0.1);
          break;
      }
    }
  }


  switch (modenumber)
  {

    case 0:
      M5.dis.fillpix(0x000000); //Fill LED display with black
      break;

    case 1:
      //Once the strobe interval has passed, if the matrix is currently all black then change it to red and vice versa
      if (currentms - lastms >= strobeinterval)
      {

        if (ledstrobestate == false)
        {
          M5.dis.fillpix(0x00FF00);
          ledstrobestate = true;
        }

        else
        {
          M5.dis.fillpix(0x000000);
          ledstrobestate = false;
        }

        lastms = currentms; //Update the last time the LEDs changed color

      }

      break;

    case 2:
      //Once the strobe interval has passed, if the matrix is currently all black then change it to white and vice versa
      if (currentms - lastms >= strobeinterval)
      {

        if (ledstrobestate == false)
        {
          M5.dis.fillpix(0xFFFFFF);
          ledstrobestate = true;
        }

        else
        {
          M5.dis.fillpix(0x000000);
          ledstrobestate = false;
        }

        lastms = currentms; //Update the last time the LEDs changed color

      }

      break;

    case 3:
      if (currentms - lastaccelms >= accelinterval) //Collect acceleration data at interval of 25 ms
      {
        if (avg1counter < 10) //Collect 10 acceleration samples for the first average
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          avgZ1 += accZ;
          avg1counter++;
        }
        else if (avg2counter < 10 && avg1counter >= 10) //Collect 10 acceleration samples for the second average after the first one is done
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          avgZ2 += accZ;
          avg2counter++;
        }
        else if (avg2counter >= 10) //Once both samples are collected calculated the averages, find the difference and reset the average sample collection
        {
          avgZ1 = avgZ1 / 10;
          avgZ2 = avgZ2 / 10;
          acceldiff = avgZ2 - avgZ1;
          avgZ1 = 0; avgZ2 = 0; avg1counter = 0; avg2counter = 0;
        }

        lastaccelms = currentms;
      }

      if (abs(acceldiff) >= 0.7) //If the absolute difference is greater than 0.9 (We can change this number until it works) then the LEDs would be solid until the acceldiff is no longer greater than 0.9
      {
        M5.dis.fillpix(0x00FF00);
      }

      else if (abs(acceldiff) < 0.7) //Else the lights strobe

      {
        if (currentms - lastms >= strobeinterval)
        {

          if (ledstrobestate == false)
          {
            M5.dis.fillpix(0x00FF00);
            ledstrobestate = true;
          }

          else
          {
            M5.dis.fillpix(0x000000);
            ledstrobestate = false;
          }


          lastms = currentms; //Update the last time the LEDs changed color

        }
      }

      break;

    case 4:

      if (currentms - lastaccelms >= accelinterval) //Collect acceleration data at interval of 25 ms
      {
        if (avg1counter < 10) //Collect 10 acceleration samples for the first average
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          avgZ1 += accZ;
          avg1counter++;
        }
        else if (avg2counter < 10 && avg1counter >= 10) //Collect 10 acceleration samples for the second average after the first one is done
        {
          M5.IMU.getAccelData(&accX, &accY, &accZ);
          avgZ2 += accZ;
          avg2counter++;
        }
        else if (avg2counter >= 10) //Once both samples are collected calculated the averages, find the difference and reset the average sample collection
        {
          avgZ1 = avgZ1 / 10;
          avgZ2 = avgZ2 / 10;
          acceldiff = avgZ2 - avgZ1;
          avgZ1 = 0; avgZ2 = 0; avg1counter = 0; avg2counter = 0;
        }

        lastaccelms = currentms;
      }

      if (abs(acceldiff) >= 0.7) //If the absolute difference is greater than 0.9 (We can change this number until it works) then the LEDs would be solid until the acceldiff is no longer greater than 0.9
      {
        M5.dis.fillpix(0xFFFFFF);
      }

      else if (abs(acceldiff) < 0.7) //Else the lights strobe

      {
        if (currentms - lastms >= strobeinterval)
        {

          if (ledstrobestate == false)
          {
            M5.dis.fillpix(0xFFFFFF);
            ledstrobestate = true;
          }

          else
          {
            M5.dis.fillpix(0x000000);
            ledstrobestate = false;
          }


          lastms = currentms; //Update the last time the LEDs changed color

        }
      }

      break;

    default:
      break;

  }

  //Cycle back to first mode after last mode
  if (modenumber >= 5)
  {
    modenumber = 0;
    if (SHOW_DESC) TextDisplay("0OFF", TXTDUR, 87, 6, 140);
    else CharacterDisplay ('0', 0.8, 87, 6, 140, true, true, 0.1);
  }

  M5.update();
}
