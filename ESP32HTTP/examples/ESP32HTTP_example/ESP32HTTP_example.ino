/*
 * EXAMPLE OF ESP32HTTP library
 * Author: migue27au --> https://github.com/migue27au
 * Last Update: 18/02/2021
 */
#include "ESP32HTTP.h"

char* server = "www.arduino.cc";
String path = "/asciilogo.txt";

char* ssid = "SSID";
char* password = "SSID PASSWORD";


HTTP http(server, HTTP_PORT);
//HTTP http(server, HTTP_PORT, true);  //If you want to see the logger output. (Serial Speed is 115200 bauds)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    /*
      0: WL_IDLE_STATUS when Wi-Fi is changing state
      1: WL_NO_SSID_AVAIL if the configured SSID cannot be reached
      3: WL_CONNECTED after connection is established
      4: WL_CONNECT_FAILED if the password is incorrect
      6: WL_DISCONNECTED if the module is not configured in station mode
    */
   
    Serial.print(WiFi.status());
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  HTTPRequest request(HTTP_GET, path);
  //HTTPRequest request(HTTP_POST, path, "your payload for post request here");
  request.setHeader("User-Agent", "ESP32");

  HTTPResponse response = http.sendRequest(request);

  int responseCode = response.getResponseCode();
  Serial.print("Responde Code: ");
  Serial.println(responseCode);
  Serial.println("Payload:");
  Serial.println(response.getPayload());
  
  if(responseCode == HTTP_RESPONSE_OK){
    Serial.println("Successfull response");
  } else{
    Serial.println("Wrong response");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
