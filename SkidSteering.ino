#include "const.h"
void setup_skid_steer(){
  pinMode(LEFT_MOTOR,OUTPUT);
  pinMode(RIGHT_MOTOR,OUTPUT);
 

  
}
void skid_steer() {
  // put your main code here, to run repeatedly:
  // RIGHT_MOTOR is right motor LEFT_MOTOR is left motor
  int l_speed;
  int r_speed;
  double turnradius;
  double Vratio;
  double Straight_speed;
  double Turning_Speed;

  int nunchucky = joy_y_axis();
  int nunchuckx = joy_x_axis();
  
  // Nunchuck to Arduino to Motor Controller

  // first, calculate forward speed, assuming no turning
  if (nunchucky > ZERO_POS_2) {
    // moving forward
    l_speed = nunchucky*0.488189+125.512;
    r_speed = l_speed;
  } else if (nunchucky < ZERO_POS_1) {
    // moving backward
    l_speed = nunchucky*0.492063+124.508;
    r_speed = l_speed;
  } else {
    // not moving
    l_speed = 0;
    r_speed = l_speed;
  }

  // first assume no turning
 
  // Turning
  turnradius = (abs(nunchuckx-127.5)/ZERO_POS_2)*MAX_TURN_RADIUS;
  Vratio = abs((turnradius + (VEHICLE_WIDTH/2))/(turnradius-(VEHICLE_WIDTH/2)));
   
  if (nunchuckx > ZERO_POS_2 && nunchucky > ZERO_POS_2) {
    // turning right, while going forwardnow
    Straight_speed = nunchucky*0.488189+125.512;
    if (Vratio*Straight_speed <= MAX_NUNCHUCK){
      l_speed = Straight_speed*Vratio;
      r_speed = Straight_speed;
    } else {
      l_speed = Straight_speed;
      r_speed = Straight_speed/Vratio;
    }
  } else if (nunchuckx < ZERO_POS_1 && nunchucky > ZERO_POS_2) {
    Straight_speed = nunchucky*0.488189+125.512;
    // turning left, while going forward
    if (Vratio*Straight_speed >= MIN_NUNCHUCK){
      l_speed = Straight_speed;
      r_speed = Straight_speed*Vratio;
    } else {
      l_speed = Straight_speed/Vratio;
      r_speed = Straight_speed;
    }
   }
    else if (nunchuckx > ZERO_POS_2 && nunchucky < ZERO_POS_1) {
    // turning right, while going backwards
    Straight_speed = nunchucky*0.492063+124.508;
    if (Vratio*Straight_speed <= MAX_NUNCHUCK){
      l_speed = Straight_speed*Vratio;
      r_speed = Straight_speed;
    } else {
      l_speed = Straight_speed;
      r_speed = Straight_speed/Vratio;
    }
  } else if (nunchuckx < ZERO_POS_1 && nunchucky < ZERO_POS_1) {
    Straight_speed = nunchucky*0.492063+124.508;
    // turning left, while going backwards
    if (Vratio*Straight_speed >= MIN_NUNCHUCK){
      l_speed = Straight_speed;
      r_speed = Straight_speed*Vratio;
    } else {
      l_speed = Straight_speed/Vratio;
      r_speed = Straight_speed;
    }
    // Steering right at a stop
  } else if (nunchuckx > ZERO_POS_2 && nunchucky <= ZERO_POS_2 && nunchucky >= ZERO_POS_1){
    Turning_Speed = abs((nunchuckx*0.488189+125.512)-187.5);
    l_speed = 187.5+Turning_Speed;
    r_speed = 187.5-Turning_Speed;    
  }
  // Steering left at a stop
  else if (nunchuckx < ZERO_POS_1 && nunchucky <= ZERO_POS_2 && nunchucky >= ZERO_POS_1){
    Turning_Speed = abs((nunchuckx*0.488189+125.512)-187.5);
    r_speed = 187.5+Turning_Speed;
    l_speed = 187.5-Turning_Speed;    
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
