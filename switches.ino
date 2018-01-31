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

#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Fonts/Picopixel.h>

Adafruit_BicolorMatrix displays[2] = { Adafruit_BicolorMatrix(), Adafruit_BicolorMatrix() };

#define BUTTON_COUNT 6
const int btnPin[BUTTON_COUNT] = { 2, 3, 9, 8, 6, 7 };
int btn[BUTTON_COUNT] = {0,0,0,0,0,0};
bool btnpressed[BUTTON_COUNT] = {false,false,false,false,false,false};

const int INIT_LIFE = 20;
int lifeTotals[2] = {0, 0};

void setup() {
  Serial.begin(9600);
  displays[0].begin(0x71);
  displays[1].begin(0x70);

  Serial.print("Ready, on 9600");

  for (int i = 0, l = BUTTON_COUNT; i < l; i++) {
    pinMode(btnPin[i], INPUT);
  }

  displays[0].setFont(&Picopixel);
  displays[0].setTextWrap(false);
  displays[0].setTextSize(1);
  displays[0].setRotation(3);
  displays[1].setFont(&Picopixel);
  displays[1].setTextWrap(false);
  displays[1].setTextSize(1);
  displays[1].setRotation(3);

  setLife(0, INIT_LIFE);
  setLife(1, INIT_LIFE);
}

void handleButtons() {
  int debounceDelay = 200;

  for (int i = 0, l = BUTTON_COUNT; i < l; i++) {
    btn[i] = digitalRead(btnPin[i]);

    if (!btnpressed[i] && btn[i] == HIGH) {
        Serial.print("PRESSED ");
        Serial.println(i);
        btnpressed[i] = true;
    } else if(btnpressed[i] && btn[i] == LOW){
        btnpressed[i] = false;
        delay(debounceDelay);
    }

  }

}

void handleDisplay() {

}

void display(int displayNum, String text, int color=LED_GREEN) {
  int pos[2] = { text.length() > 1 ? 0 : 2, 5};

  displays[displayNum].setTextColor(color);
  displays[displayNum].clear();
  displays[displayNum].setCursor(pos[0], pos[1]);
  displays[displayNum].print(text);
  displays[displayNum].writeDisplay();
}

void setLife(int playerNum, int newLife) {
  lifeTotals[playerNum] = newLife;
  int color = LED_GREEN;
  if( newLife < 10 ) color = LED_YELLOW;
  if( newLife < 5 ) color = LED_RED;
  display(playerNum, String(newLife));
}

void loop() {
  handleButtons();
  handleDisplay();
}
