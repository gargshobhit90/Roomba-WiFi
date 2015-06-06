
/*

 WiFi-controlled iRobot Roomba
 
 A simple way to control Roomba using telnet on your WiFi connection. 
 
 You can see the client's input in the serial monitor as well.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.
 
 Instructions: 
 0. Make sure Roomba is turned off. 
 1. Compile this sketch and run the Serial monitor. Arduino, with
 WiShield, attempts to connect to the specified WiFi network and gets 
 an IP address. 
 Note: At this point, Arduino and your PC can be disconnected. 
 2. Telnet to your device's IP address.
 3. Turn on Roomba. Press ? [return] to check availability.
 4. Press r to establish Arduino's connection with Roomba.
 5. Play!
 
 Circuit:
 * WiFi shield attached
 * iRobot Roomba with Create module

 created 4 June 2015
 by Shobhit Garg
 modified 6 June 2015
 by Shobhit Garg

 */

#include <SPI.h>
#include <WiFi.h>
#include <SoftwareSerial.h>

char ssid[] = "GARG.INC"; //  your network SSID (name)
char pass[] = "sgarg2000";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int ledPin = 13; //on Arduino

//For Roomba
int rxPin = 8; //connect this to Create Pin 2 
int txPin = 9; //connect this to Create Pin 1


SoftwareSerial mySerial(rxPin,txPin);

char sensorbytes[10];
int status = WL_IDLE_STATUS;

WiFiServer server(23);

boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  mySerial.begin(57600);
  digitalWrite(ledPin, HIGH); // say we're alive

  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if ( fv != "1.1.0" )
    Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // start the server:
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  
  // wait for a new client:
  WiFiClient client = server.available();
   
  // when the client sends the first byte, say hello:
  if (client) 
  {
    if (!alreadyConnected) 
    {      
      // clead out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello!");
      client.println("Turn your Roomba on.");
      client.println("Press '?' to check whether Roomba is available.");
      client.println("If available, press r to send start command to it.");
      client.println("We are now ready to play with it!");
      client.println("Directions: ");
      client.println("     w : forward");
      client.println("     s : backward");
      client.println("     a : left");
      client.println("     d : right");
      client.println("     q : stop");
      client.println("Please enter the direction to move in: ");
      
      alreadyConnected = true;
    }
  
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();

      if(thisChar == '?')
          if(mySerial.available())
             client.println("Roomba is available\n");
          else
             client.println("Roomba is not available\n");
        
       
      if(thisChar == 'r')
      {
        client.println ("Sending start command...");
        mySerial.write(128);              // START
        delay(1000);
        
        client.println ("Sending Safe Mode command...");
        mySerial.write(131);              // CONTROL
        delay(1000);
        
        digitalWrite(ledPin, LOW);      // say we've finished setup
        client.println ("Ready to go!");
        delay (1000);
      }

      int delaytime = 300; 
      
      if(thisChar == 'o')
      {
        updateSensors();
        delay(delaytime);
        client.println("Updated sensors");
      } 
      else if(thisChar == 'w')
      {
        goForward();
        delay(delaytime);
        client.println("Forward!\n");
      }
      else if(thisChar == 's')
      {
        goBackward();
        delay(delaytime);
        client.println("Backward!\n");
      }
      else if(thisChar == 'a')
      {
        spinLeft();
        delay(delaytime);
        client.println("Rotate counter-clockwise\n");
      }
      else if(thisChar == 'd')
      {
        spinRight();
        delay(delaytime);
        //Serial.write("Asking to go right\n");
        client.println("Rotate clockwise!\n");
      }
      else if(thisChar == 'q')
      {
        stopRoomba();
        delay(delaytime);
        client.println("Stop!\n");            
      }
    }
  }
}

/*
 * WiFi helper functions
 *
 */

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

/*
 * Roomba control functions
 *
 */
 
void goForward() {
  ledBlink(); // say we're starting 
  mySerial.write(137);   
  mySerial.write((byte)0x00);   
  mySerial.write(0xc8); 
  mySerial.write(0x80); 
  mySerial.write((byte)0x00); 
  Serial.print("Forward!");
}
void goBackward() {
  ledBlink(); // say we're starting 
  mySerial.write(137);   
  mySerial.write(0xff);  
  mySerial.write(0x38);
  mySerial.write(0x80);
  mySerial.write((byte)0x00);
}
void spinLeft() {
  mySerial.write(137);   
  mySerial.write((byte)0x00);  
  mySerial.write(0xc8);
  mySerial.write((byte)0x00);
  mySerial.write(0x01);   
}
void spinRight() {
  mySerial.write(137);   
  mySerial.write((byte)0x00);  
  mySerial.write(0xc8);
  mySerial.write(0xff);
  mySerial.write(0xff);   
}
void stopRoomba() {
  byte j = 0x00;
  ledBlink();
  mySerial.write(137);
  mySerial.write(j);
  mySerial.write(j);
  mySerial.write(j);
  mySerial.write(j);
}
void updateSensors() {
  mySerial.write(142);
  mySerial.write(1);  // sensor packet 1, 10 bytes
  delay(100); // wait for sensors 
  char i = 0;
  while(mySerial.available()) {
    int c = mySerial.read();
    if( c==-1 ) {
      Serial.print("Error reading sensors");
    }
    sensorbytes[i++] = c;
  }    
}

/*
 * Arduino supporting functions
 *
 */

//Blinks the LED twice
void ledBlink() {
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);
  digitalWrite(ledPin, HIGH);
  delay(100);
  digitalWrite(ledPin, LOW);
  delay(100);   
}
