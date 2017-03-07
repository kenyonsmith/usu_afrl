int count = 0;

#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include "const.h"
int LitItUp = 22;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin (115200);
  Serial.println("HAHAHAHA");
  setup_nunchuck();
  //Serial.println("Before skid steer");
  setup_skid_steer();

  Serial.println("Before app");
  setup_app();
  
  Serial.println("Before beacon");
  setup_beacon();
  
  Serial.println("Before GPS");
  setup_GPS();
  
  Serial.println("Before battery reader");
  setup_battery_reader();
  pinMode(LitItUp, OUTPUT);

  Serial1.begin(115200);
  Serial.println("Success!");
}

void loop() {
  bool nunchuck = false;
  int  x = ZERO_POS_1;
  int  y = ZERO_POS_1;
  //loop_GPS();
  //loop_battery_reader();
  delay(1000);
  
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
