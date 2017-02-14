#define MOTOR_PIN 3
int count = 0;
void setup() {
  // put your setup code here, to run once:
  setup_nunchuck();
  setup_skid_steer();
}

void loop() {
  bool nunchuck = false;
  delay(100);
  
  // first, run the Nunchuck communication
  if (nunchuck_loop()) {
    if (nunchuck == false) {
      nunchuck = true;
      nunchuck_init(0);
    }
    int y_axis = 2*(joy_y_axis() - 128);
    if (y_axis < 0)
      y_axis = -y_axis;
    if (y_axis > 255)
      y_axis = 255;
    analogWrite(MOTOR_PIN, y_axis);
    
    // next, send skid steer commands to motor controller
    skid_steer();
   //nunchuck_print ();
  } else {
    nunchuck = false;
    stop_vehicle();
  }
}
