// Load Wi-Fi library
#include <WiFi.h>

// Network credentials Here
const char* ssid     = "mata rato do casarão";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//variables to store the current LED state   s
String statePin4 = "off";
String statePin2 = "off";
String statePin5 = "off";
String statePin18 = "off";
//Output variable to GPIO pins
const int ledPin2 = 2;
const int ledPin4 = 4;
const int ledPin5 = 5;
const int ledPin18 = 18; 

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  
  pinMode(ledPin2, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin2, 0);      // turn LED off by default
  pinMode(ledPin4, OUTPUT);      // set the LED pin mode
  digitalWrite(ledPin4, 0);      // turn LED off by default
  pinMode(ledPin5, OUTPUT);   
  digitalWrite(ledPin5, 0);
  pinMode(ledPin18, OUTPUT); 
  digitalWrite(ledPin18, 0);      // turn LED off by default

  WiFi.softAP(ssid,password);
  
  // Print IP address and start web server
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.softAPIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /2/on") >= 0) {
              statePin2 = "on";
              digitalWrite(ledPin2, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /2/off") >= 0) {
              statePin2 = "off";
              digitalWrite(ledPin2, LOW);                //turns the LED off
            }
                        
            if (header.indexOf("GET /4/on") >= 0) {
              statePin4 = "on";
              digitalWrite(ledPin4, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /4/off") >= 0) {
              statePin4 = "off";
              digitalWrite(ledPin4, LOW);                //turns the LED off
            }
            
            if (header.indexOf("GET /5/on") >= 0) {
              statePin5 = "on";
              digitalWrite(ledPin5, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /5/off") >= 0) {
              statePin5 = "off";
              digitalWrite(ledPin5, LOW);                //turns the LED off
            }

            if (header.indexOf("GET /18/on") >= 0) {
              statePin18 = "on";
              digitalWrite(ledPin18, HIGH);               // turns the LED on
            } else if (header.indexOf("GET /18/off") >= 0) {
              statePin18 = "off";
              digitalWrite(ledPin18, LOW);                //turns the LED off
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            client.println("<style>html { font-family: monospace; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: yellowgreen; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 32px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: gray;}</style></head>");

            client.println("<body><h1>Gugu...Dada</h1>");
            client.println("<p>Cali...quinha</p>");

            if (statePin2 == "off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">Led Pin 2 - ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">Led Pin 2 - OFF</button></a></p>");
            }
            if (statePin4 == "off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">Led Pin 4 - ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">Led Pin 4 - OFF</button></a></p>");
            }
            if (statePin5 == "off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">Led Pin 5 - ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">Led Pin 5 - OFF</button></a></p>");
            }
             if (statePin18 == "off") {
              client.println("<p><a href=\"/18/on\"><button class=\"button\">Led Pin 18 - ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/18/off\"><button class=\"button button2\">Led Pin 18 - OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}