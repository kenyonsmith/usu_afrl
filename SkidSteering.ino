#include "const.h"
void setup_skid_steer() {
  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);



}

void skid_steer() {
  // put your main code here, to run repeatedly:
  // RIGHT_MOTOR is right motor LEFT_MOTOR is left motor
  int l_speed;
  int r_speed;
  double turnradius;
  double Vratio;
  double straight_speed;
  double turning_speed;
  int onoff = c_button();
  int nunchucky = 127;
  int nunchuckx = 128;
  int held = 0;
  // Hold c button for at least 1 second to turn nunchuck on or off
  while (onoff == 0 && held < 10)
  {
    delay(100);
    held++;
  }
  if (held < 10){
    // do nothing
  }else{
   count++;
  }
  if (count > 1) {
    count = 0;
  }
  Serial.println(count);
  if (count == 1) {
    nunchucky = joy_y_axis();
    nunchuckx = joy_x_axis();
  }

  // Nunchuck to Arduino to Motor Controller

  // first, calculate forward speed, assuming no turning
  if (nunchucky > ZERO_POS_2) {
    // moving forward
    l_speed = nunchucky * 0.488189 + 125.512;
    r_speed = l_speed;
  } else if (nunchucky < ZERO_POS_1) {
    // moving backward
    l_speed = nunchucky * 0.492063 + 124.508;
    r_speed = l_speed;
  } else {
    // not moving
    l_speed = 0;
    r_speed = l_speed;
  }

  // first assume no turning

  // Turning
  turnradius = (abs(nunchuckx - 127.5) / ZERO_POS_2) * MAX_TURN_RADIUS;
  Vratio = abs((turnradius + (VEHICLE_WIDTH / 2)) / (turnradius - (VEHICLE_WIDTH / 2)));

  if (nunchuckx > ZERO_POS_2 && nunchucky > ZERO_POS_2) {
    // turning right, while going forwardnow
    straight_speed = nunchucky * 0.488189 + 125.512;
    if (straight_speed * Vratio > MAX_VARIABLE) {
      l_speed = MAX_VARIABLE;
      r_speed = (straight_speed / (straight_speed * Vratio)) * MAX_VARIABLE;

    } else {
      l_speed = straight_speed * Vratio;
      r_speed = straight_speed;
    }
  } else if (nunchuckx < ZERO_POS_1 && nunchucky > ZERO_POS_2) {
    // turning left, while going forward
    straight_speed = nunchucky * 0.488189 + 125.512;
    if (straight_speed * Vratio > MAX_VARIABLE) {
      r_speed = MAX_VARIABLE;
      l_speed = (straight_speed / (straight_speed * Vratio)) * MAX_VARIABLE;

    } else {
      r_speed = straight_speed * Vratio;
      l_speed = straight_speed;
    }
  } else if (nunchuckx > ZERO_POS_2 && nunchucky < ZERO_POS_1) {
    // turning right, while going forwardnow
    straight_speed = nunchucky * 0.492063 + 124.508;
    if (straight_speed / Vratio < MIN_VARIABLE) {
      l_speed = MIN_VARIABLE;
      r_speed = (straight_speed / (straight_speed / Vratio)) * MIN_VARIABLE;

    }
    else {
      l_speed = straight_speed / Vratio;
      r_speed = straight_speed;
    }
  }
  else if (nunchuckx < ZERO_POS_1 && nunchucky < ZERO_POS_1) {
    // turning left, while going forward
    straight_speed = nunchucky * 0.492063 + 124.508;
    if (straight_speed / Vratio < MIN_VARIABLE) {
      r_speed = MIN_VARIABLE;
      l_speed = (straight_speed / (straight_speed / Vratio)) * MIN_VARIABLE;

    }
    else {
      r_speed = straight_speed / Vratio;
      l_speed = straight_speed;
    }
    // Steering right at a stop
  } else if (nunchuckx > ZERO_POS_2 && nunchucky <= ZERO_POS_2 && nunchucky >= ZERO_POS_1) {
    turning_speed = abs((nunchuckx * 0.488189 + 125.512) - 187.5);
    l_speed = 187.5 + turning_speed;
    r_speed = 187.5 - turning_speed;
  }
  // Steering left at a stop
  else if (nunchuckx < ZERO_POS_1 && nunchucky <= ZERO_POS_2 && nunchucky >= ZERO_POS_1) {
    turning_speed = abs((nunchuckx * 0.488189 + 125.512) - 187.5);
    r_speed = 187.5 + turning_speed;
    l_speed = 187.5 - turning_speed;
  }
  Serial.print(nunchucky);
  Serial.print(' ');
  Serial.print(l_speed);
  Serial.print(' ');
  Serial.print(r_speed);
  Serial.print(' ');
  Serial.println(Vratio);
  analogWrite(RIGHT_MOTOR, r_speed);
  analogWrite(LEFT_MOTOR, l_speed);

  /*
    // Turning Right at max speed
    else if (nunchucky == maxY && nunchuckx > zeroposx){
    speed = 255;
    turnradius = (nunchuckx/maxR)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed/Vratio);
    digitalWrite(LEFT_MOTOR, speed);
    }
    // turning right at reduced speed
    else if (nunchucky < maxY && nunchuckx > zeroposx){
    speed = (nunchucky/maxY)*255;
    turnradius = (nunchuckx/maxR)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed);
    digitalWrite(LEFT_MOTOR, speed*Vratio);
    }
    // Turning left at max speed
    else if (nunchucky == maxY && nunchuckx < zeroposx){
    speed = 255;
    turnradius = (nunchuckx/maxL)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed);
    digitalWrite(LEFT_MOTOR, speed/Vratio);
    }
    // turning left at reduced speed
    else if (nunchucky < maxY && nunchuckx < zeroposx){
    speed = (nunchucky/maxY)*255;
    turnradius = (nunchuckx/maxL)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed*Vratio);
    digitalWrite(LEFT_MOTOR, speed);
    }

  */
}
