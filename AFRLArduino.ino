#define MOTOR_PIN 3
int count = 0;
void setup() {
  // put your setup code here, to run once:
  setup_nunchuck();
  setup_skid_steer();
}

void loop() {
  delay(100);
  
  // first, run the Nunchuck communication
  if (nunchuck_loop()) {
    int y_axis = 2*(joy_y_axis() - 128);
    if (y_axis < 0)
      y_axis = -y_axis;
    if (y_axis > 255)
      y_axis = 255;
    analogWrite(MOTOR_PIN, y_axis);
   // nunchuck_print ();
  }

  // next, send skid steer commands to motor controller
  skid_steer();
}
