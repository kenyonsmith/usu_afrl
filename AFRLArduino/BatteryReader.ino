double battery;

void setup_battery_reader() {
  pinMode(A0, INPUT);

  // assume that Serial.begin(115200) has already been called
}

void loop_battery_reader() {
  double read = analogRead(A0);
  
  Serial.println(read / 34.8);
  delay(1000);
}

int batteryPercent()
{
  return (battery - 19.2) * 100 / 4.8;
}


