const int btn1 = 2;
const int btn2 = 3;

int btn1down;
bool btn1pressed = false;
int btn2down;
bool btn2pressed = false;

void setup() {
  Serial.begin(9600);

  Serial.print("Ready, on 9600");

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
}

void loop() {
  btn1down = digitalRead(btn1);
  btn2down = digitalRead(btn2);

  if (!btn1pressed && btn1down == HIGH) {
      Serial.println("PRESSED 1");
      btn1pressed = true;
  } else if(btn1down == LOW){
      btn1pressed = false;
      delay(50);
  }

  if (!btn2pressed && btn2down == HIGH) {
      Serial.println("PRESSED 2");
      btn2pressed = true;
  } else if(btn2down == LOW){
      btn2pressed = false;
      delay(50);
  }

}
