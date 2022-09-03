#include "DHT.h"        // including the library of DHT11 temperature and humidity sensor
#define DHTTYPE DHT11   // DHT 11
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define dht_dpin 0
#define HOST "192.168.1.43"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID "YPnet"            // WIFI SSID here                                   
#define WIFI_PASSWORD "112233@@"
DHT dht(dht_dpin, DHTTYPE); 

int val;
int val2;

String sendval, sendval2, postData;

void setup(void)
{ Serial.begin(115200); 
Serial.println("Communication Started \n\n");  
delay(1000);
  WiFi.mode(WIFI_STA);           
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
Serial.print("Connecting to ");
Serial.print(WIFI_SSID);
while (WiFi.status() != WL_CONNECTED) 
{ Serial.print(".");
    delay(500); }

Serial.println();
Serial.print("Connected to ");
Serial.println(WIFI_SSID);
Serial.print("IP Address is : ");
Serial.println(WiFi.localIP());    //print local IP address

delay(30);

pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  
  delay(700);

}
void loop() {
  


    float h = dht.readHumidity();
    float t = dht.readTemperature();         
    
  delay(800);
  HTTPClient http;    // http object of clas HTTPClient


// Convert integer variables to string
sendval = String(h);  
sendval2 = String(t);   

 
postData = "sendval=" + sendval + "&sendval2=" + sendval2;

http.begin("http://192.168.1.43/nodemcu/dbwrite.php");              // Connect to host where MySQL databse is hosted
http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header

  
 
int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
Serial.println("Values are, sendval = " + sendval + " and sendval2 = "+sendval2 );


// if connection eatablished then do this
if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
String webpage = http.getString();    // Get html webpage output and store it in a string
Serial.println(webpage + "\n"); 
}

// if failed to connect then return and restart

else { 
  Serial.println(httpCode); 
  Serial.println("Failed to upload values. \n"); 
  http.end(); 
  return; }


delay(3000); 
digitalWrite(LED_BUILTIN, LOW);
delay(3000);
digitalWrite(LED_BUILTIN, HIGH);
}
