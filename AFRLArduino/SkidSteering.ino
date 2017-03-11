#include "const.h"
void setup_skid_steer() {

  pinMode(LEFT_MOTOR, OUTPUT);
  pinMode(RIGHT_MOTOR, OUTPUT);
  
}
  int l_speed;
  int r_speed;
  double turnradius;
  double Vratio;
  double straight_speed;
  double turning_speed;
void stop_vehicle() {
  analogWrite(LEFT_MOTOR, 0);
  analogWrite(RIGHT_MOTOR, 0);
}

void skid_steer(int x, int y, bool cbtn) {
  // put your main code here, to run repeatedly:
  // RIGHT_MOTOR is right motor LEFT_MOTOR is left motor


  // Arduino to Motor Controller

  // first, calculate forward speed, assuming no turning
  if (y > ZERO_POS_2 && x <= ZERO_POS_2 && x >= ZERO_POS_1) {
    // moving forward
    l_speed = y * 0.488189 + 125.512;
    r_speed = l_speed;
  } else if (y < ZERO_POS_1 && x <= ZERO_POS_2 && x >= ZERO_POS_1) {
    // moving backward
    l_speed = y * 0.492063 + 124.508;
    r_speed = l_speed;
  }

  // Turning
  turnradius = ((abs(double(x) - double(ZERO_POS_1))) / double(ZERO_POS_1)) * double(MAX_TURN_RADIUS);
  Vratio = (double(turnradius) + (double(VEHICLE_WIDTH) / 2.0)) / (double(turnradius) - (double(VEHICLE_WIDTH) / 2.0));
  Serial.println(Vratio);
  if (x > ZERO_POS_2 && y > ZERO_POS_2) {
    // turning right, while going forwardnow
    straight_speed = y * 0.488189 + 125.512;
    if (straight_speed * Vratio > MAX_VARIABLE) {
      l_speed = MAX_VARIABLE;
      r_speed = (straight_speed / (straight_speed * Vratio)) * MAX_VARIABLE;

    } else {
      l_speed = straight_speed * Vratio;
      r_speed = straight_speed;
    }
  } else if (x < ZERO_POS_1 && y > ZERO_POS_2) {
    // turning left, while going forward
    straight_speed = y * 0.488189 + 125.512;
    if (straight_speed * Vratio > MAX_VARIABLE) {
      r_speed = MAX_VARIABLE;
      l_speed = (straight_speed / (straight_speed * Vratio)) * MAX_VARIABLE;

    } else {
      r_speed = straight_speed * Vratio;
      l_speed = straight_speed;
    }
  } else if (x > ZERO_POS_2 && y < ZERO_POS_1) {
    // turning right, while going forwardnow
    straight_speed = y * 0.492063 + 124.508;
    if (straight_speed / Vratio < MIN_VARIABLE) {
      l_speed = MIN_VARIABLE;
      r_speed = (straight_speed / (straight_speed / Vratio)) * MIN_VARIABLE;

    }
    else {
      l_speed = straight_speed / Vratio;
      r_speed = straight_speed;
    }
  }
  else if (x < ZERO_POS_1 && y < ZERO_POS_1) {
    // turning left, while going forward
    straight_speed = y * 0.492063 + 124.508;
    if (straight_speed / Vratio < MIN_VARIABLE) {
      r_speed = MIN_VARIABLE;
      l_speed = (straight_speed / (straight_speed / Vratio)) * MIN_VARIABLE;

    }
    else {
      r_speed = straight_speed / Vratio;
      l_speed = straight_speed;
    }
    // Steering right at a stop
  } else if (x > ZERO_POS_2 && y <= ZERO_POS_2 && y >= ZERO_POS_1) {
    turning_speed = abs((x * 0.488189 + 125.512) - 187.5);
    l_speed = 187.5 + turning_speed;
    r_speed = 187.5 - turning_speed;
  }
  // Steering left at a stop
  else if (x < ZERO_POS_1 && y <= ZERO_POS_2 && y >= ZERO_POS_1) {
    turning_speed = abs((x * 0.488189 + 125.512) - 187.5);
    r_speed = 187.5 + turning_speed;
    l_speed = 187.5 - turning_speed;
  }
  if((x >= ZERO_POS_1) && (x <= ZERO_POS_2) && (y >= ZERO_POS_1) && (y <= ZERO_POS_2)) {
    // not moving
    l_speed = 0;
    r_speed = l_speed;
  }
  Serial.print(x);
  Serial.print(' ');
  Serial.print(y);
  Serial.print(' ');
  Serial.print(l_speed);
  Serial.print(' ');
  Serial.print(r_speed);
  Serial.println(' ');
  
  analogWrite(RIGHT_MOTOR, r_speed);
  analogWrite(LEFT_MOTOR, l_speed);

  /*
    // Turning Right at max speed
    else if (y == maxY && x > zeroposx){
    speed = 255;
    turnradius = (x/maxR)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed/Vratio);
    digitalWrite(LEFT_MOTOR, speed);
    }
    // turning right at reduced speed
    else if (y < maxY && x > zeroposx){
    speed = (y/maxY)*255;
    turnradius = (x/maxR)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed);
    digitalWrite(LEFT_MOTOR, speed*Vratio);
    }
    // Turning left at max speed
    else if (y == maxY && x < zeroposx){
    speed = 255;
    turnradius = (x/maxL)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed);
    digitalWrite(LEFT_MOTOR, speed/Vratio);
    }
    // turning left at reduced speed
    else if (y < maxY && x < zeroposx){
    speed = (y/maxY)*255;
    turnradius = (x/maxL)*MAX_TURN_RADIUS;
    Vratio = (turnradius + VEHICLE_WIDTH/2)/(turnradius-VEHICLE_WIDTH/2);
    digitalWrite(RIGHT_MOTOR, speed*Vratio);
    digitalWrite(LEFT_MOTOR, speed);
    }

  */
}

