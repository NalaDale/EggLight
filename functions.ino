#ifdef DEBUG_MODE
// prints out a data report on the serial monitor
void printDebug() {
  Serial.print(F("brightness: "));
  Serial.print(brightness);
  Serial.print(F("\tthreshold: "));
  Serial.print(THRESHOLD);
  Serial.print(F("\ttime: "));
  Serial.print(t.hour);
  Serial.print(F(":"));
  Serial.print(t.min);
  Serial.print(F(":"));
  Serial.print(t.sec);
  Serial.print(F("\trelay: "));
  Serial.println(relayState);
}
#endif


// sets the pins to the right mode
void setPinModes() {
  pinMode(PHOTOCELL_PIN, INPUT_PULLUP);
}


// updates the relay state
void updateRelay() {
  // relay state is global
  relayState = ((t.hour >= ON_HOUR) && (t.hour <= OFF_HOUR) && (brightness > THRESHOLD)) ?  true : false;
  relay.write(!relayState);
  // the "not" (!) inverts the state
}
