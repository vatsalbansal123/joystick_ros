void setup() {
  Serial.begin(115200);
}

void loop() {
  int xVal = analogRead(A0);
  int yVal = analogRead(A1);

  Serial.print(xVal);
  Serial.print(",");
  Serial.println(yVal);

  delay(50); 
}