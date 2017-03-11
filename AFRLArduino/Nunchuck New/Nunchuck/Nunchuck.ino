#include <Wiichuck.h>
#include <Wire.h>

Wiichuck wii;

void setup_nunchuck() {
  Serial.begin(115200);
  wii.init();
  wii.calibrate();  // calibration
  if (wii.poll()){
    Serial.println("Device found");
  }else{
    Serial.println("No Nunchuck device found");
  }
}
uint8_t z_button(){
  return wii.buttonZ();
}
uint8_t c_button(){
  return wii.buttonC();
}
int joy_x_axis()
{
  return wii.joyX();
}
int joy_y_axis()
{
  return wii.joyY();
}
int accel_x_axis()
{
  return wii.accelX();
}
int accel_y_axis()
{
  return wii.accelY();
}
int accel_z_axis()
{
  return wii.accelZ();
}
bool nunchuck_loop() {
  if (wii.poll()) {
//    Serial.print("joy:");
//    Serial.print(wii.joyX());
//    Serial.print(", ");
//    Serial.print(wii.joyY());
//    Serial.print("  \t");
//    
//    Serial.print("accle:");
//    Serial.print(wii.accelX());
//    Serial.print(", ");
//    Serial.print(wii.accelY());
//    Serial.print(", ");
//    Serial.print(wii.accelZ());
//    Serial.print("  \t");
//    
//    Serial.print("button:");
//    Serial.print(wii.buttonC());
//    Serial.print(", ");
//    Serial.print(wii.buttonZ());
//    Serial.println("");
  }
  
  //delay(100);
  if (wii.poll())
    return true;
  else
    return false;
}
