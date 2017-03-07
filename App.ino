/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Blynk community:            http://community.blynk.cc
    Social networks:            http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use Arduino with HC-06/HC-05
  Bluetooth 2.0 Serial Port Profile (SPP) module
  to connect your project to Blynk.

  Note: This only works on Android! iOS does not support SPP :(
        You may need to pair the module with your smartphone
        via Bluetooth settings. Default pairing password is 1234

  Feel free to apply it to any other example. It's simple!

  NOTE: Bluetooth support is in beta!

  You can receive x and y coords for joystick movement within App.

  App project setup:
    Two Axis Joystick on V1 in MERGE output mode.
    MERGE mode means device will receive both x and y within 1 message
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
//#include "const.h"

int  app_x = 127;//ZERO_POS_1;
int  app_y = 127;//ZERO_POS_1;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "5b3c219d02064bf6a5d64ed986864d76"; // Samsung
char auth[] = "398ee9323e1b4d2a9ffb911a0a04107f"; // Billy's Phone
//char auth[] = "57164fa41a1046d8bc7c224b13981802"; // Moriah's Phone


SoftwareSerial SerialBLE(10, 11); // RX, TX
//int pwrBtn;     //the soft kill switch for the app
//int irBtn;      //Button turning on and off Ir system
int joyUp;      //joystick in the up directions
int joySide;    //Joystick in the side directions

double bar;     //Sliding Bar used for maximum speed

void killSwitch(){
  int pwrBtn = !digitalRead(2);
  if(pwrBtn){
    Serial.print("pwr = ");
    Serial.println(pwrBtn);
    Blynk.notify("Yaaay... button is pressed!");
  }
}
void irSwitch(){
  int irBtn = !digitalRead(3);
  if(irBtn){
    Serial.print("ir = ");
    Serial.println(irBtn);
    Blynk.notify("Yaaay... button is pressed!");
  }
}
WidgetMap myMap(V4);
void deviceLocation(){
  myMap.clear();
  int index = 1;
  float lat = 51.5074;
  float lon = 115.1278;
  myMap.location(index, lat, lon, "value");
  
}
BLYNK_WRITE(V2){
  bar = param.asDouble();

  Serial.print("Bar = ");
  Serial.println(bar);
}

BLYNK_WRITE(V1) {
  joyUp = (param[1].asInt()-127)*(bar/100)+127;
  joySide = param[0].asInt();
 
  // Do something with x and y
  Serial.print("X = ");
  Serial.print(joySide);
  Serial.print("; Y = ");
  Serial.println(joyUp);
  app_x = joySide;
  app_y = joyUp;
 
}

int app_x_axis() {
  return app_x;
}

int app_y_axis() {
  return app_y;
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);
  //pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), killSwitch, CHANGE);
  //pinMode(1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), irSwitch, CHANGE);
  Serial.println("Waiting for connections...");
}

void loop()
{
  
  Blynk.run();
  Blynk.virtualWrite(V3,3.14);
  deviceLocation();
  //Serial.println("This will make me happy if I see this.");
  
}
