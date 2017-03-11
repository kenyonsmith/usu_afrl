// read out a Wii Nunchuck controller 
// adapted to work with wireless Nunchuck controllers of third party vendors by Michael Dreher <michael@5dot1.de>

// adapt to your hardware config
#define POWER_VIA_PORT_C2_C3 1    // use port pins port C2 and C3 as power supply of the Nunchuck (direct plug using wiichuck adapter) 
#define DEBUG_RCV_TEL 1

#define USE_NEW_WAY_INIT 1        // use "The New Way" of initialization <http://wiibrew.org/wiki/Wiimote#The_New_Way>
#define WII_IDENT_LEN ((byte)6)
#define WII_TELEGRAM_LEN ((byte)6)
#define WII_NUNCHUCK_TWI_ADR ((byte)0x52)

#include <Wire.h>
#include <string.h>
#include <utility\twi.h>
#undef int
#include <stdio.h>


uint8_t outbuf[WII_TELEGRAM_LEN];            // array to store arduino output
int cnt = 0;
int ledPin = 13;

void setup_nunchuck ()
{
  outbuf[0] = ZERO_POS_1;
  outbuf[1] = ZERO_POS_1;

#ifdef POWER_VIA_PORT_C2_C3    // power supply of the Nunchuck via port C2 and C3
 PORTC &=~ _BV(PORTC2);
 PORTC |=  _BV(PORTC3);
 DDRC |= _BV(PORTC2) | _BV(PORTC3);  // make outputs
 delay(100);  // wait for things to stabilize
#endif

 Wire.begin(); // initialize i2c
 Wire.beginTransmission(WII_NUNCHUCK_TWI_ADR);
 if (Wire.endTransmission() == 0) {
  // we found the device
  Serial.println("Device found");
  
  // we need to switch the TWI speed, because the nunchuck uses Fast-TWI
  // normally set in hardware\libraries\Wire\utility\twi.c twi_init(
  // this is the way of doing it without modifying the original files
 #define TWI_FREQ_NUNCHUCK 400000L
  TWBR = ((16000000 / TWI_FREQ_NUNCHUCK) - 16) / 2;

  nunchuck_init(0); // send the initialization handshake

  // display the identification bytes, must be "00 00 A4 20 00 00" for the Nunchuck
  byte i;
 
  if(readControllerIdent(outbuf) == 0)
  {
    Serial.print("Ident=");
    for (i = 0; i < WII_TELEGRAM_LEN; i++) 
    {
      Serial.print(outbuf[i], HEX);
      Serial.print(' ');
    }
    Serial.println();
  }

  Serial.println("Finished setup");  
 } else {
  Serial.println("No Nunchuck device found");
 }
}

// params:
//   timeout: abort when timeout (in ms) expires, 0 for unlimited timeout
//   return:  0 == ok, 1 == timeout
byte nunchuck_init (unsigned short timeout)
{
   
 byte rc = 1;

#ifndef USE_NEW_WAY_INIT
 // look at <http://wiibrew.org/wiki/Wiimote#The_Old_Way> at "The Old Way"
 Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);      // transmit to device 0x52
 Wire.write (0x40);            // sends memory address
 Wire.write (0x00);            // sends sent a zero.  
 Wire.endTransmission ();      // stop transmitting

#else
 // disable encryption
 // look at <http://wiibrew.org/wiki/Wiimote#The_New_Way> at "The New Way"

 unsigned long time = millis();
 do
 {
   Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);      // transmit to device 0x52
   Wire.write (0xF0);            // sends memory address
   Wire.write (0x55);            // sends data.
     
   if(Wire.endTransmission() == 0) // stop transmitting
   {
     Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);      // transmit to device 0x52
     Wire.write (0xFB);            // sends memory address
     Wire.write (0x00);            // sends sent a zero.  
     
     if(Wire.endTransmission () == 0)      // stop transmitting
     {
       rc = 0;
       
     }
   }
   
 } 
 while (rc != 0 && (!timeout || ((millis() - time) < timeout)));
#endif

 return rc;
}


// params:
//   ident [out]: pointer to buffer where 6 bytes of identification is stored. Buffer must be at least 6 bytes long.
//                A list of possible identifications can be found here: <http://wiibrew.org/wiki/Wiimote#The_New_Way>
//   return:  0 == ok, 1 == error
byte readControllerIdent(byte* pIdent)
{
 byte rc = 1;

 // read identification
 Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);      // transmit to device 0x52
 Wire.write (0xFA);            // sends memory address of ident in controller
 if(Wire.endTransmission () == 0)      // stop transmitting
 {
   byte i;
   Wire.requestFrom (WII_NUNCHUCK_TWI_ADR, WII_TELEGRAM_LEN);      // request data from nunchuck
   for (i = 0; (i < WII_TELEGRAM_LEN) && Wire.available (); i++) 
   {
     pIdent[i] = Wire.read();      // receive byte as an integer
   }
   if(i == WII_TELEGRAM_LEN)
   {
     rc = 0;
   }
 }
 return rc;
}

void clearTwiInputBuffer(void)
{
 // clear the receive buffer from any partial data
 while( Wire.available ())
   Wire.read ();
}


void send_zero ()
{
 // I don't know why, but it only works correct when doing this exactly 3 times
 // otherwise only each 3rd call reads data from the controller (cnt will be 0 the other times)
 for(byte i = 0; i < 3; i++)
 {
   Wire.beginTransmission (WII_NUNCHUCK_TWI_ADR);      // transmit to device 0x52
   Wire.write (0x00);            // sends one byte
   Wire.endTransmission ();      // stop transmitting
   //delay(1);
 }
}

 // byte outbuf[5] contains bits for z and c buttons
 // it also contains the least significant bits for the accelerometer data
 // so we have to check each bit of byte outbuf[5]
 
bool z_button()
{
  if ((outbuf[5] >> 0) & 1)
    return true;
  else
    return false;
}

bool c_button()
{
  if ((outbuf[5] >> 1) & 1)
    return true;
  else
    return false;
}

int joy_x_axis()
{
  return outbuf[0];
}

int joy_y_axis()
{
  return outbuf[1];
}

int accel_x_axis()
{
  int accel_x_axis = outbuf[2] * 2 * 2; 
  if ((outbuf[5] >> 2) & 1)
    accel_x_axis += 2;
  if ((outbuf[5] >> 3) & 1)
    accel_x_axis += 1;
  return accel_x_axis;
}

int accel_y_axis()
{
  int accel_y_axis = outbuf[3] * 2 * 2; 
  if ((outbuf[5] >> 4) & 1)
    accel_y_axis += 2;
  if ((outbuf[5] >> 5) & 1)
    accel_y_axis += 1;
  return accel_y_axis;
}

int accel_z_axis()
{
  int accel_z_axis = outbuf[4] * 2 * 2; 
  if ((outbuf[5] >> 6) & 1)
    accel_z_axis += 2;
  if ((outbuf[5] >> 7) & 1)
    accel_z_axis += 1;
  return accel_z_axis;
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
void nunchuck_print ()
{
 int joy_x = joy_x_axis();
 int joy_y = joy_y_axis();
 int accel_x = accel_x_axis();
 int accel_y = accel_y_axis();
 int accel_z = accel_z_axis();
 int c_btn = c_button();
 int z_btn = z_button();
 
// Serial.print (joy_x, DEC);
// Serial.print ("\t");
//
// Serial.print (joy_y, DEC);
// Serial.print ("\t");
//
// Serial.print (accel_x, DEC);
// Serial.print ("\t");
//
// Serial.print (accel_y, DEC);
// Serial.print ("\t");
//
// Serial.print (accel_z, DEC);
// Serial.print ("\t");
//
// Serial.print (z_btn, DEC);
// Serial.print ("\t");
//
// Serial.print (c_btn, DEC);
// Serial.print ("\t");
//
// Serial.print ("\r\n");
}

// Decode data format that original Nunchuck uses with old init sequence. This never worked with
// other controllers (e.g. wireless Nunchuck from other vendors)
char nunchuk_decode_byte (char x)
{
#ifndef USE_NEW_WAY_INIT
 x = (x ^ 0x17) + 0x17;
#endif
 return x;
}

// this function returns true if a signal was received from the Nunchuck
bool nunchuck_loop ()
{
 // delay (100);

 // request data from manual controller
 send_zero (); // send the request for next bytes
 Wire.requestFrom (WII_NUNCHUCK_TWI_ADR, WII_TELEGRAM_LEN);      // request data from nunchuck

 // get data from manual controller
 for (cnt = 0; (cnt < WII_TELEGRAM_LEN) && Wire.available (); cnt++) 
 {
   outbuf[cnt] = nunchuk_decode_byte (Wire.read ());      // receive byte as an integer
   digitalWrite (ledPin, HIGH);      // sets the LED on
 }

 // print some debugging data
 //Serial.print("avail=");
 //Serial.print(Wire.available(), DEC);
// Serial.print("  cnt=");
 //Serial.println(cnt, DEC);
 clearTwiInputBuffer();

 // If we recieved the 6 bytes, then return true
 if (cnt >= WII_TELEGRAM_LEN)
   return true;
 else
   return false;
}

