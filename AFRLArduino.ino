int count = 0;

#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include "const.h"
int LitItUp = 22;

void setup() {
  // put your setup code here, to run once:
  
  setup_nunchuck();
  setup_skid_steer();
  setup_app();
  setup_beacon();
  setup_GPS();
  setup_battery_reader();
  Serial1.begin(115200);
  pinMode(LitItUp, OUTPUT);
}

void loop() {
  bool nunchuck = false;
  int  x = ZERO_POS_1;
  int  y = ZERO_POS_1;
  loop_GPS();
  loop_battery_reader();
  delay(100);
  
  // first, run the Nunchuck communication
  if (nunchuck_loop()) {
	// we got a response
    if (nunchuck == false) {
	  // initialize the Nunchuck if it just got plugged in
      nunchuck = true;
      nunchuck_init(0);
    }
	
	  x = joy_x_axis();
	  y = joy_y_axis();
  } else nunchuck = false;
  
  if ((x == ZERO_POS_1) || (x == ZERO_POS_2) && ((y == ZERO_POS_1) || (y == ZERO_POS_2))) {
    // check for input from app
    loop_app();
    x = app_x_axis();
    y = app_y_axis();
  }
  
	if ((x > ZERO_POS_2) || (x < ZERO_POS_1) || (y > ZERO_POS_2) || (y < ZERO_POS_1)) {
	  skid_steer(x,y,c_button());
	} else {
		// this means that no input was received from the manual controller
    
    x = 127;
    y = 127;
    skid_steer(x,y,c_button());
  }

  if (Serial1.available())
  {Serial.print(Serial1.read());
  
    char c;
    c = Serial1.read();
    switch(c)
    {
    case '1'://DIE ROBOT DIE (Switch Open)
      digitalWrite(LitItUp, HIGH);//trade LitItUp for relay signal pin in arduino code.
      break;
    case '0'://ROBOT IS FREE (Switch Closed)
      digitalWrite(LitItUp, LOW);
      break;
    }

  }
}
