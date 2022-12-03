 #include "M5Atom.h"
#include "TextDisplay.h"

// CHARACTER DISPLAY DURATION 
#define TXTDUR 1.25

// TEMPERATURE GENERATION
#define LOWERBOUND 9
#define UPPERBOUND 12   // For generating data points between LOWERBOUND and UPPERBOUND for testing purposes using GenerateTemps()

// TEMPERATURE SAMPLES
#define SAMPLES 24      // Amount of data points that can fit on screen

// ACCELEROMETER MODE CHANGING
uint8_t modenumber = 0;
float temp = 0, currenttemp = 0;
float accX = 0, accY = 0, accZ = 0, avgZ = 0, avgX = 0, avgTemp=0,tempsum = 0 ;
int numOfDataPts = 10;
int tiltDirection = 0; //device in the centre
// Check if the item is already tilting
int isTilting = 0;

// TEMPEARATURE READING
unsigned long lasttempms = 0;
const unsigned long avgtempinterval =  1200000; // Every 20 minutes update the temp 
const int intervalsToCombine = 3;               // Each 3 values are averaged, avoiding magic number 3
const int tempreadingsnum = SAMPLES * intervalsToCombine;
float tempreadings[tempreadingsnum] = {};
int avgtempcounter = 0;

// TEMPERATURE GRAPHING
float temperatureArray[SAMPLES] = {}; // Store 24-value temperature readings (avg every intervalsToCombine intervals)
int recordedSamples = 0;
// Temperature marks for color scale understanding
// [0] is the lower scale bound, and [1] is the higher scale bound
float temperatureMarks[5] = {};
// Range of temperature scale
float temperatureScaleRange = 0;
unsigned char* output5x5 = new unsigned char[77];

// TEMPERATURE UNITS
int unitType = 0; // 0 for Celsius, 1 for Fahrenheit, 2 for Kelvin
float UnitConversion(float value, int unitType, std::string& unit) {
  if (unitType == 0) {unit = "'C"; return value;}
  else if (unitType == 1) {unit = "F"; return (value * 9 / 5) + 32;}
  else {unit = "K"; return value + 273.15;}
}

// Function to record data
void updateTempData()
{
  unsigned long currentms = millis(); //Stores the current time the program has been running for in milliseconds
        
  if (currentms - lasttempms >= avgtempinterval)
  {
      M5.IMU.getTempData(&currenttemp);
      currenttemp -= 15;
      if (avgtempcounter < tempreadingsnum) //Add data to array every twenty minutes until array is full
      {
           tempreadings[avgtempcounter] = currenttemp;
           currenttemp = 0;
           avgtempcounter++;
      }

      else if(avgtempcounter >= tempreadingsnum)
      {
           avgtempcounter = 0; //Once array is full reset the counter
           //Update tempreadings array within same interval as counter is reset so interval isn't skipped
           tempreadings[avgtempcounter] = currenttemp; 
           currenttemp = 0;
           avgtempcounter++;
      }

      lasttempms = currentms;
  }
}

void updateTiltData() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  //obtain modified moving averages in the X and Z directions
  avgX = ( ((numOfDataPts - 1) * avgX) + accX ) / numOfDataPts;
  avgZ = ( ((numOfDataPts - 1) * avgZ) + accZ ) / numOfDataPts;
  // Serial.println(String(avgX) + "     " + String(avgZ));

  if (avgX < -0.5 && isTilting != -1){
    isTilting = -1;
    modenumber--;
    if (modenumber == 0) modenumber = 5;
    Serial.println("Tilt left");
  } else if (avgX > 0.5 && isTilting != 1){
    isTilting = 1;
    modenumber++;
    if (modenumber == 6) modenumber = 1;
    Serial.println("Tilt right");
  } else if (abs(avgX) < 0.5 && isTilting != 0) {
    isTilting = 0;
    Serial.println("Tilt stops");
  }
  
  /*if (abs(avgX) >= 0.5 && !isTilting) //tilting device
  {
    isTilting = true;
    if (avgX < -0.5) {
      tiltDirection = 1; //tilt left
      modenumber--;
      // M5.dis.fillpix(0x00FF00);
    }
    if (avgX > 0.5) {
      tiltDirection = 2; //tilt right
      modenumber++;
      // M5.dis.fillpix(0xFFFFFF);
    }
  } else if (!(abs(avgX >= 0.5)) && isTilting) {
    isTilting = false;
  }*/

  /*if (M5.Btn.wasPressed()) //or (avgZ < 0.7 && abs(avgX) < 0.3)) //or screen tapped)
  {
    modenumber = 1;
  }*/

  if (avgZ > 0.7 && abs(avgX) < 0.3 && modenumber != 0) //user turns the device down
  {
    modenumber = 0;
  }
}

void setup() {
  M5.begin(true, false, true);
  M5.IMU.Init();
  Serial.begin(115200);
  modenumber = 0;
}

// Buffer string for text displays
std::string buffr = "";
std::string unit = "";
char buff[10] = "";

void Updater() {
  updateTiltData();
  updateTempData();
  M5.update();
}

void loop() {
  switch (modenumber)
  {
    case 0:
      M5.dis.fillpix(0x000000); //Fill LED display with black
      // M5.IMU.getTempData(&temp); //record temp data
      M5.update();
      while (!M5.Btn.wasPressed()) {
        // updateTiltData();
        updateTempData();
        M5.update();
        // Serial.print("Is in mode 0");
      }
      modenumber = 1;
      break;
    case 1:
      CharacterDisplay ('1', 0.2, 100, 0 , 100, false, false, 0, true);
      while (modenumber == 1 && !M5.Btn.wasPressed()) Updater();
      if (modenumber != 1) break;
      M5.IMU.getTempData(&temp); //record temp data
      buffr = dtostrf(UnitConversion(temp, unitType, unit), 3, 4, buff);
      TextDisplay(buffr + unit, TXTDUR, 87, 6, 140);
      /* while (modenumber == 1) {
        updateTiltData();
        M5.update();
      }*/
      break;

    case 2:
      CharacterDisplay ('2', 0.2, 100, 0 , 100, false, false, 0, true);
      while (modenumber == 2 && !M5.Btn.wasPressed()) Updater();
      if (modenumber != 2) break;
      for (int i = 0; i<=avgtempcounter; i++) //Sum all the temperatures in the array
      {
           tempsum += tempreadings[avgtempcounter];
      }
      
      avgTemp = tempsum/(avgtempcounter + 1); //Average all the temperatures within the array
      tempsum = 0;
      
      buffr = dtostrf(UnitConversion(avgTemp, unitType, unit), 3, 4, buff);
      TextDisplay(buffr + unit, TXTDUR, 87, 6, 140);
      break;

    case 3: {
      CharacterDisplay ('3', 0.2, 100, 0 , 100, false, false, 0, true);
      while (modenumber == 3 && !M5.Btn.wasPressed()) Updater();
      if (modenumber != 3) break;
      
      DataConversion();
      GenerateTemperatureScale();
      
      int places = DecimalCounter(temperatureMarks[0]);
      if (places > DecimalCounter(temperatureMarks[4])) places = DecimalCounter(temperatureMarks[4]);
      
      unsigned char R = 0, G = 0;
      std::string prefix = "Mark 0: ";
      for (int i = 0; i < 5; i++) {
        GenerateColor((temperatureMarks[i] - temperatureMarks[0]) * 2 / temperatureScaleRange, &R, &G);
        buffr = dtostrf(UnitConversion(temperatureMarks[i], unitType, unit), 3, places + 2, buff);
        TextDisplay(prefix + buffr + unit, TXTDUR, R, G, 0);
        prefix[5]++;
      }
      
      M5.IMU.getTempData(&temp); //record temp data
      GenerateColor((temp - temperatureMarks[0]) * 2 / temperatureScaleRange, &R, &G);
      buffr = dtostrf(UnitConversion(temp, unitType, unit), 3, 4, buff);
      TextDisplay(buffr + unit, TXTDUR, R, G, 0);
      break;
    }
    case 4: {
      CharacterDisplay ('4', 0.2, 100, 0 , 100, false, false, 0, true);
      while (modenumber == 4 && !M5.Btn.wasPressed()) Updater();
      if (modenumber != 4) break;
      
      DataConversion();
      GenerateTemperatureScale();
      CreateTemperatureGraph(output5x5);

      int places = DecimalCounter(temperatureMarks[0]);
      if (places > DecimalCounter(temperatureMarks[4])) places = DecimalCounter(temperatureMarks[4]);

      unsigned char R = 0, G = 0;
      std::string prefix = "Mark 0: ";
      for (int i = 0; i < 5; i++) {
        GenerateColor((temperatureMarks[i] - temperatureMarks[0]) * 2 / temperatureScaleRange, &R, &G);
        buffr = dtostrf(UnitConversion(temperatureMarks[i], unitType, unit), 3, places + 2, buff);
        TextDisplay(prefix + buffr + unit, TXTDUR, R, G, 0);
        prefix[5]++;
      }

      delay(5);
      for (int i = 0; i < SAMPLES; i++) {
        // Serial.println(String(i) + ": " + String((int)output5x5[i * 3 + 2]) + ": " + String((int)output5x5[i * 3 + 3]) + ": " + String((int)output5x5[i * 3 + 4])); 
      }
      M5.dis.displaybuff(output5x5, 0, 0);
      M5.update();
      while (modenumber == 4 && !M5.Btn.wasPressed()) Updater();
      break;
    }
    case 5: {
      CharacterDisplay ('5', 0.2, 100, 0 , 100, false, false, 0, true);
      while (modenumber == 5 && !M5.Btn.wasPressed()) Updater();
      if (modenumber != 5) break;
      char uSym = 'K';
      while (modenumber == 5) {
        if (unitType == 0) uSym = 'C';
        else if (unitType == 1) uSym = 'F';
        CharacterDisplay (uSym, 0.2, 100, 0 , 100, false, false, 0, true);
        M5.update();
        while (modenumber == 5 && !M5.Btn.wasPressed()) Updater();
        if (modenumber ==  5) {
          unitType++;
          if (unitType == 3) unitType = 0;
        }
      }
      break;
    }
    default:
      modenumber = 0;
      break;
  }
  updateTiltData();
  updateTempData();
  M5.update();
}

/*
 * TEMPERATURE COLOR SCALING & GRAPHING
 * Reading temp data from temperature array
 */
 
// Generate sampleCount temperatures between LOWERBOUND and UPPERBOUND for testing purposes
// Pass in float array tempData and amount of samples sampleCount
void GenerateTemps(float* tempData, int sampleCount) {
 for (int i = 0; i < sampleCount; i++) tempData[i] = rand() % int(100 * (UPPERBOUND - LOWERBOUND)) * 1.0 / 100 + LOWERBOUND;
}

// Conversion between tempreadings and temperatureArray
// tempreadings overwrites old readings, when temperatureArray stores data oldest first
void DataConversion() {
  // Check if overwriting has begun or not (if final value in array is (or very close to) 0)
  float threshold = 0.0001;
  bool hasOverwritten = abs(tempreadings[tempreadingsnum - 1]) > threshold;

  // If not overwritten, the data is not divisible by intervalsToCombine
  if (!hasOverwritten) {
    float avg = 0;
    recordedSamples = 0;
    // Copy newest data
    for (int i = 0; i < avgtempcounter / intervalsToCombine; i++) {
      avg = 0;
      for (int j = 0; j < intervalsToCombine; j++) avg += tempreadings[i * intervalsToCombine + j];
      avg /= intervalsToCombine;
      if (avgtempcounter % intervalsToCombine > 0) 
        temperatureArray[SAMPLES - (avgtempcounter / intervalsToCombine) - 1 + i] = avg;
      else temperatureArray[SAMPLES - (avgtempcounter / intervalsToCombine) + i] = avg;
      recordedSamples++;
    }
    if (avgtempcounter % intervalsToCombine > 0) {
      avg = 0;
      for (int j = avgtempcounter - avgtempcounter % intervalsToCombine; j < avgtempcounter; j++) {
        avg += tempreadings[j];
      }
      avg /= avgtempcounter % intervalsToCombine;
      temperatureArray[SAMPLES - 1] = avg;
      recordedSamples++;
    }
  }
  // Otherwise the data can be added more easily
  else {
    float avg = 0;
    int counter = 0;
    recordedSamples = SAMPLES;

    // Read the newest portion first
    for (int i = avgtempcounter - 1; i >= 0; i--) {
      avg += tempreadings[i];
      counter++;
      if (counter % intervalsToCombine == 0) {
        temperatureArray[SAMPLES - counter / intervalsToCombine] = avg / intervalsToCombine;
        avg = 0;
      }
    }

    // Then the old non-overwritten portion
    for (int i = tempreadingsnum - 1; i >= avgtempcounter; i--) {
      avg += tempreadings[i];
      counter++;
      if (counter % intervalsToCombine == 0) {
        temperatureArray[SAMPLES - counter / intervalsToCombine] = avg / intervalsToCombine;
        avg = 0;
      }
    }
  }
}

// Float t changes from 0 to 2
// 0 means the value is lowest in the range (cool temperature)
// 1 means the value is at middle (normal temperature)
// 2 means the value is highest (hot temperature)
// Only varies from green (0) - lime - yellow (1) - orange - red (2)
void GenerateColor(float t, unsigned char* R, unsigned char* G) {
  if (t < 1 && t >= 0) {
    *R = round(t * (-255 * t + 510));
    *G = 255;
  }
  else if (t == 1) {
    *R = 255;
    *G = 255;
  }
  else if (t > 1 && t <= 2) {
    *R = 255;
    *G = round(255 * t * (2 - t));
  } 
  else if (t < 0) {
    *R = 0;
    *G = 255;
  }
  else if (t > 2) {
    *R = 255;
    *G = 0;
  }
}

// Create temperature scale, put into temperatureMarks
void GenerateTemperatureScale() {
  float lowerBound = temperatureArray[0];
  float upperBound = temperatureArray[0];
  if (recordedSamples > 1) for (int i = SAMPLES - recordedSamples; i < SAMPLES; i++) {
    if (temperatureArray[i] < lowerBound) lowerBound = temperatureArray[i];
    else if (temperatureArray[i] > upperBound) upperBound = temperatureArray[i];
  }
  // cout << upperBound - lowerBound << endl;

  // Evaluate the relative size of difference between upper and lower bound,
  // floored to get nicely round results
  float differenceSizeRelativeTo10Powers = log10(upperBound - lowerBound);
  int flooredDifference = floor(differenceSizeRelativeTo10Powers);

  // This difference is used to eliminate smaller decimal places
  double multiplier = pow(10, -flooredDifference);
  float multipliedLowerBound = lowerBound * multiplier;
  float multipliedUpperBound = upperBound * multiplier;
  
  // Floor to get the final scale bounds
  temperatureMarks[0] = floor(multipliedLowerBound) / multiplier;
  temperatureMarks[4] = ceil(multipliedUpperBound) / multiplier;

  // Create range
  temperatureScaleRange = temperatureMarks[4] - temperatureMarks[0];

  // Populates the rest of temperatureMarks
  temperatureMarks[2] = (temperatureMarks[0] + temperatureMarks[4]) / 2;
  temperatureMarks[1] = (temperatureMarks[0] + temperatureMarks[2]) / 2;
  temperatureMarks[3] = (temperatureMarks[2] + temperatureMarks[4]) / 2;

  // Display
  // for (int i = 0; i < 5; i++) cout << temperatureMarks[i] << ", ";
  // cout << endl;
}

// Create temperature graph
// Dependent on: temperatureMarks, temperatureScaleRange
// output5x5: array of 77 indexes
void CreateTemperatureGraph(unsigned char* output5x5) {
  output5x5[0] = 5;
  output5x5[1] = 5;
  unsigned char R = 0, G = 0;
  for (int i = 0; i < SAMPLES - recordedSamples; i++) {
    // cout << "R: " << int(R) << ", G: " << int(G) << endl;
    output5x5[2 + 3 * i] = 0;
    output5x5[2 + 3 * i + 1] = 0;
    output5x5[2 + 3 * i + 2] = 0;
  }
  for (int i = SAMPLES - recordedSamples; i < SAMPLES; i++) {
    // Get color of temperature
    GenerateColor((temperatureArray[i] - temperatureMarks[0]) * 2 / temperatureScaleRange, &R, &G);
    // cout << "R: " << int(R) << ", G: " << int(G) << endl;
    output5x5[2 + 3 * i] = R;
    output5x5[2 + 3 * i + 1] = G;
    output5x5[2 + 3 * i + 2] = 0;
  }

  // White pixel for orientation
  output5x5[74] = 220;
  output5x5[75] = 220;
  output5x5[76] = 220;
}

// Count the number of decimal places in a POSITIVE double
int DecimalCounter(double a) {
  // Stave off non-decimal places
  a -= floor(a);

  double threshold = 0.0001;
  int decimalPlaces = 0;
  while (a > threshold) {
    a *= 10;
    a -= floor(a);
    decimalPlaces++;
  }
  return decimalPlaces;
}
