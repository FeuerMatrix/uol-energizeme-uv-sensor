
#include <SparkFun_VEML6075_Arduino_Library.h>
#include <WiFiManager.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <SD.h>
#define chipSelect 16
//sets up the library for getting timestamps
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
VEML6075 uv;
File file;


void setup() {
  //sets up the automatic wifi management
  //If in a new Network, connect to it via WiFi and give it password information for the network you want to connect it with
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  
  timeClient.begin();


  //sets up Serial Monitor
  Serial.begin(9600);
  delay(2000);
  Serial.println("Booting Up");

  //sets up the sd card
  if(SD.begin(chipSelect)) {
    Serial.println("SD comm open");
  }
  
  //sets up uv sensor
  Wire.begin();
  Serial.println("Starting UV Sensor Connection");
  if (uv.begin() == false) {
    Serial.println("Unable to communicate with VEML6075.");
    while (1) {
      delay(1000);
      Serial.println("Error");
    } 
  }
}

void loop() {
  timeClient.update();
  file = SD.open("testDir/data.txt", FILE_WRITE);
  if (file) {
    Serial.println("Writing Value:");
    Serial.println(String(timeClient.getEpochTime()) + ": " + String(uv.a()) + ", " + String(uv.b()));
    file.println(String(timeClient.getEpochTime()));
    file.println(String(uv.a()));
    file.println(String(uv.b()));
    file.close();
  } else {
    Serial.println("Opening file failed");
  }
  delay(900000);
}
