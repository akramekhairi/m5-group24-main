#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
#include "M5Atom.h"
#include "TextDisplay.h"

// credentials for accessing WiFi
const char *ssid = "group24AP";
const char *password = "superlab";

//to switch between device modes
int deviceMode = 0;

//counter mode
int clickCounter = 0;
std::string stringCounter="";

//spinning mode
float accX = 0, accY = 0, accZ = 0, avgX = 0, avgY = 0, avgZ = 0;
int numOfDataPts = 10;
unsigned long startTime = 0, endTime = 0;
unsigned long spinDuration = 0;

//art mode
const unsigned char image[77] =
{
  0x05,
  0x05,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, //
  0xff, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xff, 0x00, 0x00, //
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, //
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, //
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //
};

long var = 0;



WiFiServer server(80);

void setup() {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  // configuring server
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();
  Serial.println("Server started");

  //setting up m5 atom
  M5.begin(true, false, true);
  M5.IMU.Init();

}


void loop() {

  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        //Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // html and javascript code sent to the browser

        // Check to see client request
        if (currentLine.endsWith("GET /CLICK")) {
          Serial.println("Click Counter Mode");
          client.println("Click Counter Mode");
          deviceMode = 1;
          deviceDisplay();

        }
        if (currentLine.endsWith("GET /SPIN")) {
          Serial.println("Spin Mode");
          client.println("Spin Mode");
          deviceMode = 2;
          deviceDisplay();

        }
      }
      if (currentLine.endsWith("GET /ART")) {
        Serial.println("Art Mode");
        client.println("Art Mode");
        deviceMode = 3;
        deviceDisplay();

      }

    }


    if (currentLine.endsWith("GET /home")) {
      Serial.println("Home");


      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<head>");
      client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
      client.println("<script type=\"text/javascript\">");
      client.println("window.onload = function() {");
      client.println("httpGetAsync(\"/clickCounter\");");
      client.println("httpGetAsync(\"/spinDuration\");");
      client.println("function httpGetAsync(theUrl) { ");
      client.println("var xmlHttp = new XMLHttpRequest();");
      client.println("var frequency_val = 1000;");
      client.println("xmlHttp.onreadystatechange = function() { ");
      client.println("if (xmlHttp.readyState == 4 && xmlHttp.status == 200)");
      client.println("document.getElementById(\"clickCounter\").innerHTML = xmlHttp.responseText; }");
      client.println("document.getElementById(\"spinDuration\").innerHTML = xmlHttp.responseText; }");
      client.println("xmlHttp.open(\"GET\", theUrl, true); ");
      client.println("xmlHttp.send(null)");
      client.println("setTimeout(function(){httpGetAsync(\"/clickCounter\")}, frequency_val); } }");
      client.println("setTimeout(function(){httpGetAsync(\"/spinDuration\")}, frequency_val); } }");
      client.println("</script>");
      client.println("</head>");

      client.println("<body>");
      client.println("<h1> Welcome to your Fidget Device Tracker! </h1>");
      client.println("<h2 style=\"font-size: 22px; display: inline; color: Blue\">Click Counter: </h2> ");
      client.println("<br></br>");
      client.println(clickCounter);
      client.println("<br></br>");
      client.print("<p style=\"font-size: 12px; display: inline; color:black\">Click <a href=\"/CLICK\">here</a> to enter click mode on your device </p> <br>");
      client.println("<h2 style=\"font-size: 22px; display: inline; color: Blue\">Spin Duration: </h2> ");
      client.println("<br></br>");
      client.println(spinDuration);
      client.println("<br></br>");
      client.print("<p style=\"font-size: 12px; display: inline; color:black\">Click <a href=\"/SPIN\">here</a> to enter spin mode on your device </p> <br>");
      client.println("<h2 style=\"font-size: 22px; display: inline; color: Blue\">Art Mode: </h2> ");
      client.println("<br></br>");
      client.print("<p style=\"font-size: 12px; display: inline; color:black\">Click <a href=\"/ART\">here</a> to enter art mode on your device </p> <br>");
      client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");

      client.println("</body>");
      client.println("</html>");
    }
  }




  // close the connection:
  client.stop();
  //Serial.println("Client Disconnected.");
}

void deviceDisplay()
{
  switch (deviceMode)
  {
    case 0:
      break;

    case 1:

      M5.update();
      if (M5.Btn.wasPressed())
      {
        clickCounter++;
        stringCounter="0";
        stringCounter[0]+=clickCounter;
        TextDisplay(stringCounter, 1.25, 87, 6, 140);
        Serial.print( "Button Pressed \n");
        M5.update();
      }

      break;

    case 2:
      while (deviceMode == 2)
      {
        M5.IMU.getAccelData(&accX, &accY, &accZ);
        avgX = ( ((numOfDataPts - 1) * avgX) + accX ) / numOfDataPts; //modified moving average
        avgY = ( ((numOfDataPts - 1) * avgY) + accY ) / numOfDataPts;
        avgZ = ( ((numOfDataPts - 1) * avgZ) + accZ ) / numOfDataPts;
        if (abs(avgX) >= 0.5 or abs (avgY) >= 0.5 or abs(avgZ) >= 0.5)
        {
          startTime = millis();
          Serial.print( "Device spinning \n");
          M5.dis.fillpix(0xFFFFFF);
          M5.update();
        }
        Serial.print( "Spinning stopped \n");
        endTime = millis();
        spinDuration = endTime - startTime;
        Serial.print(spinDuration);
      }
      break;

    case 3:
      Serial.print( "Entering Art Mode \n");
      displayArt();
      break;

  }
}


// function to display pixel art image
void displayArt() {
  for (int i = 0; i < 2; i++) {
    for (int n = 0; n < 10; n++) {
      M5.update();
      M5.dis.displaybuff((uint8_t *)image);

    }
  }
}
