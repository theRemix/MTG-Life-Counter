const int inPin = 2;

bool pressed = false;
int reading;

void setup() {
  Serial.begin(9600);

  Serial.print("Ready, on 9600");

  pinMode(inPin, INPUT);
}

void loop() {
  reading = digitalRead(inPin);

  if (reading == HIGH && !pressed) {
    pressed = true;
    Serial.println("PRESSED 2");
  } else {
    pressed = false;
    delay(50);
  }
}
