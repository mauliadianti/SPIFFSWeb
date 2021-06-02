#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include "SPIFFS.h"


WiFiUDP udp;
const char *ssid = "ESP32wifi";
const char *password = "123456789";
const char *udpAddress = "192.168.4.184"; //ip address client 

AsyncWebServer serverWeb(80);
const int udpPort=2390;
 
byte buff[50]; 

String Data;
String Temperature;//T = Variable Data Temperature
String Voltage;//V = Variable Data Current
String Current;//C = Variable Data Voltage


void setup() {
  Serial.begin(115200);               // Used for serial debugging
  Serial.print("Setting AP (Access Point)");
  Serial.println(); 
  WiFi.softAP(ssid,password);
 
 
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  udp.begin(udpPort);

    SPIFFS.begin() ; 
  
  //memanggil web html 
  serverWeb.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/Server6.html", "text/html" );
  });
  
  serverWeb.on("/Temperature", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", Temperature);
  });
  serverWeb.on("/Voltage", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", Voltage);
  });
  serverWeb.on("/Current", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", Current);
  });
  serverWeb.begin();
}

void loop() {
  //request yang dikirim ke client
  uint8_t buff[50] = "Data" ;
  Serial.print("Sending     : "); 
  Serial.println((char*) buff);
  
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buff, 50);
  udp.endPacket();
  memset(buff, 0, 50);
  udp.parsePacket();

if (udp.read(buff, 50) > 0){
   Serial.print("I received  : ");
   Serial.println(Data);
   Data = (char*)buff;
   String dataIn = String (Data); 
    
    
    Temperature = getValue(dataIn, ',' , 0);
    Voltage     = getValue(dataIn, ',' , 1);
    Current     = getValue(dataIn, ',' , 2); 

    Serial.print("Temperature : ");
    Serial.println(Temperature);
    Serial.print("Voltage     : "); 
    Serial.println(Voltage); 
    Serial.print("Current     : ");
    Serial.println(Current);
    
} 
delay(1000);
//}else {
// delay(1000); 
// Serial.println("Requested time out");}  
} 

String getValue( String data, char separator, int index)
 {
  int found = 0;
  int strIndex[] = {0, -1}; //loop
  int maxIndex = data.length() ; 

  for (int i = 0; i<= maxIndex && found <= index; i++){
    if (data.charAt(i) == separator || i == maxIndex) {
      found++; 
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ?  i + 1 : i; 
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
 }

 
