int count = 0;

#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include "const.h"
int LitItUp = 22;
double lat = 41.742670;
double lon = -111.806592;
bool app_paired = false;

void setup() {
  // put your setup code here, to run once:

  Serial.begin (115200);
  setup_nunchuck();
  //Serial.println("Before skid steer");
  setup_skid_steer();

  init_check_pairing();
  if (check_pairing()) {
    app_paired = true;
    Serial.println("App connecting... now!");
    setup_app();
  }

  Serial.println("Before beacon");
  setup_beacon();

  Serial.println("Before GPS");
  setup_GPS();

  Serial.println("Before battery reader");
  setup_battery_reader();
  pinMode(LitItUp, OUTPUT);

  //Serial1.begin(115200);
 //Serial.println("Success!");
}
double decimalDegrees(double nmeaCoord) {
  uint16_t wholeDegrees = 0.01 * nmeaCoord;
  return wholeDegrees + (nmeaCoord - 100.0 * wholeDegrees) / 60.0;
}
bool nunchuck = false;
int  x = ZERO_POS_1;
int  y = ZERO_POS_1;


void loop() {
  

  loop_GPS();
  if (fix()) {
    lon = decimalDegrees(Long());
    lat = decimalDegrees(Lat());
    if (Latchar() == 'S') {
      lat = -lat;
    }
    if (Longchar() == 'W') {
      lon = -lon;
    }
    
    //deviceLocation(41.753072,-111.837857);
  }
  loop_app(lat, lon);
  //loop_battery_reader();
  delay(100);

  // first, run the Nunchuck communication
  if (nunchuck_loop()) {
    // we got a response
    if (nunchuck == false) {
      // initialize the Nunchuck if it just got plugged in
      setup_nunchuck();
      nunchuck = true;
    }

    x = joy_x_axis();
    y = joy_y_axis();
  } else {
    nunchuck = false;
  }

  if ((x >= ZERO_POS_1) && (x <= ZERO_POS_2) && (y >= ZERO_POS_1) && (y <= ZERO_POS_2)) {
    // check for input from app, because we got nothing from Nunchuck
    app_paired = double_check_pairing(check_pairing());
    if (app_paired) {
      if(app_x_axis() == 0 || app_y_axis() == 0){
        x = 127;
        y = 127;
      } else {
      x = app_x_axis();
      y = app_y_axis();
      }
    }
  } 

  if ((x >= ZERO_POS_1) && (x <= ZERO_POS_2) && (y >= ZERO_POS_1) && (y <= ZERO_POS_2) && irSwitch() == 1) {
    // check for input from beacon, because we got nothing from Nunchuck or from app
    x = beacon_x_axis();
    y = beacon_y_axis();
  }

  if ((x > ZERO_POS_2) || (x < ZERO_POS_1) || (y > ZERO_POS_2) || (y < ZERO_POS_1)) {
    skid_steer(x, y, c_button());
  } else if (x > 255 || x < 0 || y > 255 || y < 0) {
    // This means that there was an error in the input recieved.

    x = 127;
    y = 127;
    skid_steer(x, y, c_button());
  } else {
    x = 127;
    y = 127;
    skid_steer(x, y, c_button());
  }
  //  Serial.println(irSwitch());
  //
  Serial.print("NX: ");
  Serial.print(joy_x_axis());
  Serial.print(", NY: ");
  Serial.print(joy_y_axis());
  Serial.print(", AX: ");
  Serial.print(app_x_axis());
  Serial.print(", AY: ");
  Serial.print(app_y_axis());
  Serial.print(", X: ");
  Serial.print(x);
  Serial.print(", Y: ");
  Serial.print(y);
  Serial.println(" :)");

  //  Serial.println(Latchar());
  //  Serial.println(Longchar());
  //  Serial.println(lat);
  //  Serial.println(lon);
//  if (Serial1.available())
//  { //Serial.print(Serial1.read());
//
//    char c;
//    c = Serial1.read();
//    switch (c)
//    {
//      case '1'://DIE ROBOT DIE (Switch Open)
//        digitalWrite(LitItUp, HIGH);//trade LitItUp for relay signal pin in arduino code.
//        break;
//      case '0'://ROBOT IS FREE (Switch Closed)
//        digitalWrite(LitItUp, LOW);
//        break;
//    }
//
//  }
//  if (pwrSwitch() == 1) {
//    digitalWrite(LitItUp, HIGH);
//  } else if (pwrSwitch == 0) {
//    digitalWrite(LitItUp, LOW);
//  }
}
