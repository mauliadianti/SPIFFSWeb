#include "WiFi.h"
//#include "ESPAsyncWebServer.h"
#include <WiFi.h>
#include <WiFiUdp.h>
//#include "SPIFFS.h"


WiFiUDP udp;
const char *ssid = "ESP32wifi";
const char *password = "123456789";
const char *udpAddress = "192.168.4.184"; //ip address client 

AsyncWebServer serverWeb(80);
const int udpPort=2390;
 
String Data;
String A;//A = Var data yg dikirim dri client
byte buff[50]; 

String Temperature;//T = Variable Data Temperature
String Voltage;//V = Variable Data Current
String Current;//C = Variable Data Voltage


void setup() {
  Serial.begin(115200);               // Used for serial debugging
  Serial.print("Setting AP (Access Point)…");
  //Serial.println("Program running...");

  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  udp.begin(udpPort);
}

void loop() {
  //request yang dikirim ke client
  uint8_t buff[50] = "Data" ;
//  delay(1000); 
  
  A = (char*) buff;
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buff, 50);
  udp.endPacket();
  memset(buff, 0, 50);
  udp.parsePacket();

  
    udp.parsePacket();
    String dataIn;
if (udp.read(buff, 50) > 0) { 
   Serial.print("I received : ");
   Data = (char*)buff;
//   dataIn = String(Data); 
   Serial.println(Data);
    }

    
    Temperature = getValue(Data, ',', 0);
    Voltage     = getValue(Data, ',', 1);
    Current     = getValue(Data, ',', 2); 

    Serial.print("Temperature : ");
    Serial.println(Temperature);
    Serial.print("Voltage: "); 
    Serial.println(Voltage); 
    Serial.print("Current: ");
    Serial.println(Current); 
    
 delay(1000);   
} 
void abcde()
{ 
    byte arrayData[60];
    while (Serial.available())
  {
    for (int i=0; i<=59 ; i++) 
  {
    arrayData[i] = udp.read(buff, 50);
    
    Data = (char*)arrayData;
    Temperature = getValue(Data, ',', 0);
    Voltage     = getValue(Data, ',', 1);
    Current     = getValue(Data, ',', 2); 
  }
//    if (([i] == 
  }
    Serial.print("Temperature : ");
    Serial.println(Temperature);
    Serial.print("Voltage: "); 
    Serial.println(Voltage); 
    Serial.print("Current: ");
    Serial.println(Current); 
 }

String getValue( String data, char separator, int index)
 {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() ; 

  for (int i = 0; i<= maxIndex && found <= index; i++){
    if (data.charAt(i) == separator || i == maxIndex) {
      found++; 
      strIndex[0] = strIndex[1] + 1;
//      strIndex[1] = strIndex[2] + 1; 
      strIndex[1] = (i == maxIndex) ?  i + 1 : i; 
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
 }
