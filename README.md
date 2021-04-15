# ESP8266_MQTT_Speaker
Hacksmith Vlog #Null
Simple MQTT Speaker utilizing "dfplayer mini" to play preloaded sounds

The code has been simplified from the original and is ment to be used as a helper for your own original work.
Most functionality is still within the project.

## Required Components
  * 1 - ESP8266 (Your choice but project used D1 Mini)
  * 2 - dfplayer mini
  * 3 - 4ohm or 8ohm speaker (4 watt MAX)
  * 4 - SD Card (Keep this under 32GB)

## Required Infrastructure
  * 1 - MQTT - Self Hosted < I would chose this. Use it with a raspberry pie or VM. Google it :P
  * 2 - MQTT - Cloud Hosted ([Something like this](https://www.hivemq.com/mqtt-cloud-broker/))
  
## Required Arduino Capable Board Libraries
  * 1 - ESP8266 ([Here is how to install it](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/))

## Required Module Libraries
  * 1 - Arduino.h
  * 2 - ESP8266WiFi.h (Comes with ESP8266 Board Library)
  * 3 - SoftwareSerial.h
  * 4 - PubSubClient.h (MQTT)
  * 5 - DFrobotDFPlayerMini.h (For the audio Player)
  * 6 - StringSplitter.h


## MQTT command for Playing file
 * Topic : Hacksmith/Speaker/announce_speaker/1/announcement
 * Msg   : 30^01^002
 * Description : This will > Set Volume to 30 > Go into folder #1 > Play Audio clip labeled 001

## MQTT command for Changing Volume
 * Topic : Hacksmith/Speaker/announce_speaker/1/volume
 * Msg   : 0 - 30
 * Description : Sets the volume from 0(0%) to 30(100%)

## MQTT command for Pausing Audio Playback
 * Topic : Hacksmith/Speaker/announce_speaker/1/control
 * Msg   : pause
 * Description : Pauses current audio playback


This is not the same as having a PA (Although it can be used as one for pre-baked msg's).
This is for simple everyday announcements like doorbells, lunchtimes, deliveries...etc.

Join the Hacksmith Discord If you wish to ask any questions or have any cool ideas.
[Join Discord Here!](https://discord.gg/thehacksmith)
