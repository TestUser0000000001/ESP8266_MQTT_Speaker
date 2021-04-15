#include <ESP8266WiFi.h>              //Esp Board Wifi Library
#include "Arduino.h"                  //Arduino Library (for some things)
#include "SoftwareSerial.h"           //This lets you use other pins besides RX/TX for serial print
#include <PubSubClient.h>             //MQTT module
#include "DFRobotDFPlayerMini.h"      //dfplayer library
#include <StringSplitter.h>           //This module is for MQTT messages. This will allow you to send the messages as a string to do 1 big action or many small actions.


//Name of Device
const char* dfplayer_mini_info = "Hacksmith GitHub - dfplayer Speaker";

// Update these with values suitable for your wifi network.
const char* ssid = "YOURSHERE";
const char* password = "YOURSHERE";

//Update values for your MQTT server
//This example uses a basic username/password to secure MQTT - You may remove it if you want.
const char* mqtt_server = "YOURSHERE";
const char* mqtt_user = "YOURSHERE";
const char* mqtt_pass = "YOURSHERE";
//MQTT Topics (If you are new, no touchy the following 2 topics)
const char* announce_connected = "Hacksmith/Speaker/Connected";
const char* subscribe_topic = "Hacksmith/Speaker/announce_speaker/1/#";
//Variable String for Storing MQTT last MSG
String mqtt_full_payload = "";


//Setup reffernces for Libraries
SoftwareSerial mySoftwareSerial(D5,D6); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

WiFiClient espClient;
PubSubClient client(espClient);

#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//This is a visual Devider, yes you may remove it if you dont like it *********************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup_wifi() {
  delay(10);
  //We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //This while loop keeps happening until wifi connection has been made
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Takes the incoming MQTT string and Splits it using the ^ as a point for split.Think of it like a Java style Object.
//Then it takes the elements of the message and applies them to the dfplayer command
void playFolder_mqtt() {
  yield(); //You need this for some esp8266 devices...why? IDK
  Serial.println();
  Serial.print("String In : ");
  Serial.print(mqtt_full_payload);
  Serial.println();

    StringSplitter *splitter = new StringSplitter(mqtt_full_payload, '^', 3);
    int itemCount = splitter->getItemCount();
    
  Serial.print(itemCount);
  Serial.println();

  String vol_mqtt = splitter->getItemAtIndex(0);
  String folder_mqtt = splitter->getItemAtIndex(1);
  String sound_mqtt = splitter->getItemAtIndex(2);

  Serial.println(vol_mqtt);
  Serial.println(folder_mqtt);
  Serial.println(sound_mqtt);

  myDFPlayer.volume(vol_mqtt.toInt());
  myDFPlayer.playFolder(folder_mqtt.toInt(), sound_mqtt.toInt());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Changes the player volume even during audio playback
void volume_mqtt() {
  yield(); 
  Serial.println();
  Serial.print("String In : ");
  Serial.print(mqtt_full_payload);
  Serial.println();
  
  myDFPlayer.volume(mqtt_full_payload.toInt());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//This section does actions when the device gets an MQTT msg to a specific channel
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();

  String sort_topic = topic;
  mqtt_full_payload = "";

  //Stitches the MQTT incoming msg into 1 string
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    char thepayload = payload[i];
    mqtt_full_payload.concat(thepayload);
    }

  if (sort_topic == "Hacksmith/Speaker/announce_speaker/1/announcement") {
    Serial.println();
    Serial.print("Announcement arrived");
    playFolder_mqtt();
  }
  
  if (sort_topic == "Hacksmith/Speaker/announce_speaker/1/control") {
    Serial.println();
    Serial.print("Control arrived");
   
    if (mqtt_full_payload == "pause") {
      //Set lights to play basketball
      myDFPlayer.pause();
    }
  }

  if (sort_topic == "Hacksmith/Speaker/announce_speaker/1/volume") {
    Serial.println();
    Serial.print("Control arrived");
    volume_mqtt();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//This Part is for MQTT and Wifi reconnection incase if EITHER has been droped
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = dfplayer_mini_info;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(announce_connected, clientId.c_str());
      client.publish(announce_connected, String(dfplayer_mini_info).c_str());
      // ... and resubscribe
      client.subscribe(subscribe_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Main Setup of ESP and also default settings load
void setup()
{
  mySoftwareSerial.begin(9600); //Serial for dfplayer
  Serial.begin(115200);         //Serial for human beings

  setup_wifi(); //Call this module
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println();
  Serial.println(dfplayer_mini_info);
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  //if (!myDFPlayer.begin(mySoftwareSerial, false)) {  //Use This bit of code If Comm not working
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  Serial.println(F("Default : Serial Timeout 500ms"));

  //----Set volume----
  myDFPlayer.volume(20);  //Set volume value (0~30).
  Serial.println(F("Default : Volume 10 (30max)"));

  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  Serial.println(F("Default : EQ = Normal"));

  //----Set device use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  Serial.println(F("Default : Read From SD"));

  //----Read imformation----
  Serial.println(myDFPlayer.readState()); //read mp3 state
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.println(myDFPlayer.readEQ()); //read EQ setting
  Serial.println(myDFPlayer.readFileCounts()); //read&print all file counts in SD card
  Serial.println(myDFPlayer.readCurrentFileNumber()); //read&print current play file number
  Serial.println(myDFPlayer.readFileCountsInFolder(1)); //read&print file counts in folder SD:/01
  Serial.println(myDFPlayer.readFileCountsInFolder(2)); //read&print file counts in folder SD:/02
  Serial.println(myDFPlayer.readFileCountsInFolder(3)); //read&print file counts in folder SD:/03

  //----Boot Up sound (If you got one)----
  //myDFPlayer.playFolder(01, 001);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void loop()
{
  client.loop(); //Checks MQTT connection
  delay(5);

  if (!client.connected()) { //Checks if still connected to wifi, if not reconnect
    reconnect();
  }

  static unsigned long timer = millis();

  if (myDFPlayer.available()) {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//*****************************************************************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//dfplayer mini - Notification msg printouts

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
  
