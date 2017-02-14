int count = 0;
void setup() {
  // put your setup code here, to run once:
  setup_nunchuck();
  setup_skid_steer();
}

void loop() {
  bool nunchuck = false;
  int  x = ZERO_POS_1;
  int  y = ZERO_POS_1;

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
  }
	if ((x > ZERO_POS_2) || (x < ZERO_POS_1) || (y > ZERO_POS_2) || (y < ZERO_POS_1)) {
	  skid_steer(x,y,c_button());
	} else {
		// this means that no input was received from the manual controller
    nunchuck = false;
    stop_vehicle();
  }
}
