/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPLnzqSDWQs"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "UvV_BvNPUH8snqrbRuLXek7VFwUgJkI_"
#define LINE_TOKEN  "f4Wb0N98myDTRYJjETulfRcBYj1y9ScNndxoZQBqaY6"
#include <TridentTD_LineNotify.h>
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial

#include <NTPClient.h>
#include <Servo.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
Servo myservo;
char auth[] = BLYNK_AUTH_TOKEN;
const long utcOffsetInSeconds = 25200;
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Your Wifi";
char pass[] = "Your Password Wifi";
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", utcOffsetInSeconds);

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.

void setup()
{
  // Debug console
  myservo.attach(5);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  Blynk.syncAll();

}
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();

  if (pinValue == 1) {
    Serial.println(pinValue);
    servo();
    linenotify();
  }
 
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}
void servo(){
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
  myservo.write(0);
  delay(1000);
}
void linenotify() {
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());
  
  WiFi.begin(ssid,pass);
  Serial.printf("WiFi connecting to %s\n",  ssid);
  while(WiFi.status() != WL_CONNECTED) { Serial.print("."); delay(400); }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());  

  // กำหนด Line Token
  LINE.setToken(LINE_TOKEN);

  // ตัวอย่างส่งข้อความ
  LINE.notify("ไอมนุษย์ข้าได้รับอาหารแล้ว");
}
void loop()
{
  printtime();
  if((timeClient.getHours() == 7 && timeClient.getMinutes() == 30) || (timeClient.getHours() == 17 && timeClient.getMinutes() == 30) ){
    servo();
    linenotify();
    delay(60000);
    }
  Blynk.run();

}
void printtime(){
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);
}

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
