# ESP8266_MQTT_Speaker
Simple MQTT Speaker utilizing "dfplayer mini" to play preloaded sounds

The code has been simplified from the original and is ment to be used as a helper for your own original work.
Most functionality is still within the project.

## Required Components
  * 1 - ESP8266 (Your choice but project used D1 Mini)
  * 2 - dfplayer mini
  * 3 - 4ohm or 8ohm speaker (4 watt MAX)
  * 4 - SD Card (Keep this under 32GB)
  
## Required Arduino Capable Board Libraries
  * 1 - ESP8266 ([Here is how to install it](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/))

## Required Module Libraries
  * 1 - Arduino.h
  * 2 - ESP8266WiFi.h (Comes with ESP8266 Board Library)
  * 3 - SoftwareSerial.h
  * 4 - PubSubClient.h (MQTT)
  * 5 - DFrobotDFPlayerMini.h (For the audio Player)
  * 6 - StringSplitter.h

This is not the same as having a PA (Although it can be used as one for pre-baked msg's).
This is for simple everyday announcements like doorbells, lunchtimes, deliveries...etc.

Join the Hacksmith Discord If you wish to ask any questions or have any cool ideas.
[Join Discord Here!](https://discord.gg/thehacksmith)
