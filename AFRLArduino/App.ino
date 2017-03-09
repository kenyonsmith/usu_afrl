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
//#define BLYNK_PRINT Serial


#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include "const.h"

int  app_x = ZERO_POS_1;
int  app_y = ZERO_POS_1;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "5b3c219d02064bf6a5d64ed986864d76"; // Samsung
char auth[] = "398ee9323e1b4d2a9ffb911a0a04107f"; // Billy's Phone
//char auth[] = "57164fa41a1046d8bc7c224b13981802"; // Moriah's Phone
//char auth[] = "1cf0a97c2cdb445ba218caeacd8d5e18"; // Kenyon's Phone

int pairCheckVal = 0;


SoftwareSerial SerialBLE(10, 11); // RX, TX
int pwrBtn;     //the soft kill switch for the app
int irBtn;      //Button turning on and off Ir system
int joyUp;      //joystick in the up directions
int joySide;    //Joystick in the side directions

double bar;     //Sliding Bar used for maximum speed

WidgetMap myMap(V4);
void deviceLocation(double lat,double lon){
  myMap.clear();
  int index = 1;

  myMap.location(index, lat, lon, "Vehicle");
  Serial.println("Test");
  
}
BLYNK_WRITE(V2){
  bar = param.asDouble();

  Serial.print("Bar = ");
  Serial.println(bar);
}
BLYNK_WRITE(V5){
  irBtn = param.asInt();
}
BLYNK_WRITE(V6){
  pwrBtn = param.asInt();
}
BLYNK_WRITE(V1) {
  joyUp = param[1].asInt();//-127)*(bar/100)+127;
  joySide = param[0].asInt();
 
  // Do something with x and y
/*  Serial.print("X = ");
  Serial.print(joySide);
  Serial.print("; Y = ");
  Serial.println(joyUp);
*/  app_x = joySide;
  app_y = joyUp;
}

void init_check_pairing() {
  pinMode(A0, INPUT);
}

bool check_light() {
  if (analogRead(A0) > 500)
    return true;
  else
    return false;
}

bool check_pairing() {
  bool result = true;
  for (int i = 0; i < 4; i++) {
    int val = analogRead(A0);
    if (check_light() == false) {
      result = false;
      i = 4;
    }
    delay(50);
  }
  return result;
}

bool double_check_pairing(bool app_paired) {
  if (app_paired == true) {
    // make sure it's still paired (possibly faulty because will attempt to connect even
    // if light is flashing and currently off)
    app_paired = check_light();
//    if (app_paired == false) {
//      Serial.println("Connection lost");
//    } else {
//      Serial.println("Connection still there!");
//    }
  } else {
    if (pairCheckVal >= 10) {
      // check for pairing every 10th call, about every second
      pairCheckVal = 0;
      app_paired = check_pairing();
//      if (app_paired == true) {
//        Serial.println("Connection found!");
//      }
    } else ++pairCheckVal;
  }
  return app_paired;
}

int irSwitch() {
  return irBtn;
}
int pwrSwitch() {
  return pwrBtn;
}
int app_x_axis() {
  return app_x;
}

int app_y_axis() {
  return app_y;
}

void setup_app()
{
  // Debug console
  // Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);
  //pinMode(2, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(2), killSwitch, CHANGE);
  //pinMode(1, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(3), irSwitch, CHANGE);
  Serial.println("Waiting for connections...");
}

void loop_app(double lat, double lon)
{
  
  Blynk.run();
  Blynk.virtualWrite(V3,3.14);
  deviceLocation(lat,lon);
  //pwrBtn = digitalRead(2);
  //irBtn = digitalRead(3);
  //Serial.println(irBtn);
  //Serial.println("This will make me happy if I see this.");
  
}
