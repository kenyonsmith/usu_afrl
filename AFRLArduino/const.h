#ifndef AFRL_CONST
  #define AFRL_CONST
  const double    VEHICLE_LENGTH = 0.5588;     // defining vehicle's length from center to each wheel (in)
  const double    VEHICLE_WIDTH = 0.519911; // defining vehicle's width from center to each wheel (in)
  const double    MAX_TURN_RADIUS = 4.2672;

  const int    LEFT_MOTOR = 3;
  const int    RIGHT_MOTOR = 2;
  
  const int    MAX_STOP = 74;         // from 0-74, the vehicle will be stopped
  const int	   MAX_REVERSE = 125;     // from 75-125, the vehicle will be in full reverse
  const int    MIN_VARIABLE = 124;
  const int    MAX_VARIABLE = 250;    // from 126-250, the vehicle will be variable speed (187.5 is stop)
  const int    MAX_FORWARD = 255;     // from 251-255, the vehicle will be full forward
  const double CONTROLLER_STOP = (MAX_VARIABLE - MAX_REVERSE) / 2;
  const double VARIABLE_RANGE = MAX_VARIABLE - CONTROLLER_STOP;

  const int    ZERO_POS_1 = 127;
  const int    ZERO_POS_2 = 128;

  const int    MAX_NUNCHUCK = 255;
  const int    MIN_NUNCHUCK = 0;
  
#endif

