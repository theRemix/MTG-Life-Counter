/*

BUTTONS

+---+ +---+ +---+ +---+
| 1 | | 5 | | 6 | | 3 |
+---+ +---+ +---+ +---+

+---+ +---+ +---+ +---+
| 2 | |   | |   | | 4 |
+---+ +---+ +---+ +---+

PINS

+---+ +---+ +---+ +---+
| 2 | | 6 | | 7 | | 9 |
+---+ +---+ +---+ +---+

+---+ +---+ +---+ +---+
| 3 | |   | |   | | 8 |
+---+ +---+ +---+ +---+

*/
const int btn1 = 2;
const int btn2 = 3;
const int btn3 = 9;
const int btn4 = 8;
const int btn5 = 6;
const int btn6 = 7;

int btn1down;
int btn2down;
int btn3down;
int btn4down;
int btn5down;
int btn6down;
bool btn1pressed = false;
bool btn2pressed = false;
bool btn3pressed = false;
bool btn4pressed = false;
bool btn5pressed = false;
bool btn6pressed = false;

void setup() {
  Serial.begin(9600);

  Serial.print("Ready, on 9600");

  pinMode(btn1, INPUT);
  pinMode(btn2, INPUT);
  pinMode(btn3, INPUT);
  pinMode(btn4, INPUT);
  pinMode(btn5, INPUT);
  pinMode(btn6, INPUT);
}

void loop() {
  int debounceDelay = 200;
  btn1down = digitalRead(btn1);
  btn2down = digitalRead(btn2);
  btn3down = digitalRead(btn3);
  btn4down = digitalRead(btn4);
  btn5down = digitalRead(btn5);
  btn6down = digitalRead(btn6);

  if (!btn1pressed && btn1down == HIGH) {
      Serial.println("PRESSED 1");
      btn1pressed = true;
  } else if(btn1pressed && btn1down == LOW){
      btn1pressed = false;
      delay(debounceDelay);
  }

  if (!btn2pressed && btn2down == HIGH) {
      Serial.println("PRESSED 2");
      btn2pressed = true;
  } else if(btn2pressed && btn2down == LOW){
      btn2pressed = false;
      delay(debounceDelay);
  }

  if (!btn3pressed && btn3down == HIGH) {
      Serial.println("PRESSED 3");
      btn3pressed = true;
  } else if(btn3pressed && btn3down == LOW){
      btn3pressed = false;
      delay(debounceDelay);
  }

  if (!btn4pressed && btn4down == HIGH) {
      Serial.println("PRESSED 4");
      btn4pressed = true;
  } else if(btn4pressed && btn4down == LOW){
      btn4pressed = false;
      delay(debounceDelay);
  }

  if (!btn5pressed && btn5down == HIGH) {
      Serial.println("PRESSED 5");
      btn5pressed = true;
  } else if(btn5pressed && btn5down == LOW){
      btn5pressed = false;
      delay(debounceDelay);
  }

  if (!btn6pressed && btn6down == HIGH) {
      Serial.println("PRESSED 6");
      btn6pressed = true;
  } else if(btn6pressed && btn6down == LOW){
      btn6pressed = false;
      delay(debounceDelay);
  }

}
