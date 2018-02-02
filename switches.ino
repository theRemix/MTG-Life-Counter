/*

BUTTONS

+---+ +---+ +---+ +---+
| 0 | |   | |   | | 2 |
+---+ +---+ +---+ +---+

+---+ +---+ +---+ +---+
| 1 | | 4 | | 5 | | 3 |
+---+ +---+ +---+ +---+

PINS

+---+ +---+ +---+ +---+
| 2 | |   | |   | | 9 |
+---+ +---+ +---+ +---+

+---+ +---+ +---+ +---+
| 3 | | 6 | | 7 | | 8 |
+---+ +---+ +---+ +---+

*/

#include <Wire.h>
#include <Adafruit_LEDBackpack.h>
#include <Fonts/Picopixel.h>

Adafruit_BicolorMatrix displays[2] = { Adafruit_BicolorMatrix(), Adafruit_BicolorMatrix() };

static const uint8_t PROGMEM
  smile_bmp[] =
    { B00111100,
      B01000010,
      B10100101,
      B10000001,
      B10100101,
      B10011001,
      B01000010,
      B00111100 },
  sad_bmp[] =
    { B00111100,
      B01000010,
      B10100101,
      B10000001,
      B10011001,
      B10100101,
      B01000010,
      B00111100 };

#define BUTTON_COUNT 6
#define SONIC_COUNT 2
#define INIT_LIFE 20
#define KNOCK_DISTANCE 14

// states
#define INIT 0
#define CHOOSING_PLAYER 1
#define GAME_ON 2
#define GAME_OVER 3

const int btnPin[BUTTON_COUNT] = { 2, 3, 9, 8, 6, 7 };
const int sonicTriggerPins[SONIC_COUNT] = {12, 10};
const int sonicEchoPins[SONIC_COUNT] = {13, 11};

int  btn[BUTTON_COUNT] = {0,0,0,0,0,0};
bool btnpressedLastVal[BUTTON_COUNT] = {false,false,false,false,false,false};
bool btnpressed[BUTTON_COUNT] = {false,false,false,false,false,false};

long sonicDurations[SONIC_COUNT] = {0, 0};
int  sonicDistances[SONIC_COUNT] = {0, 0};

int lifeTotals[2] = {0, 0};
int currentPlayerTurn = 0;
int state = INIT;

void setup() {
  Serial.begin(9600);
  displays[0].begin(0x71);
  displays[1].begin(0x70);

  Serial.print("Ready, on 9600");
  state = INIT;

  for (int i = 0, l = SONIC_COUNT; i < l; i++) {
    pinMode(sonicTriggerPins[i], OUTPUT);
    pinMode(sonicEchoPins[i], INPUT);
  }

  for (int i = 0, l = BUTTON_COUNT; i < l; i++) {
    pinMode(btnPin[i], INPUT);
  }

  for (int i = 0, l = 2; i < l; i++) {
    displays[i].setFont(&Picopixel);
    displays[i].setTextWrap(false);
    displays[i].setTextSize(1);
    displays[i].setRotation(3);
  }

  randomizeFirstPlayer();
}

void clearDisplays() {
  for (int i = 0; i < 2; i++) {
    displays[i].clear();
    displays[i].writeDisplay();
  }
}

int resetButtonCombo[] = {0, 0};
void handleReset( int b1, int b2 ) {
  resetButtonCombo[0] = resetButtonCombo[0] + b1;
  resetButtonCombo[1] = resetButtonCombo[1] + b2;

  if( resetButtonCombo[0] > 0 && resetButtonCombo[1] > 0 ) {
    doReset();
  }
}

void randomizeFirstPlayer() {
  state = CHOOSING_PLAYER;
  clearDisplays();

  int speed = 5;
  const int minCycles = 6;
  const int maxCycles = 16;
  int randomCycles = random(minCycles, maxCycles);
  bool direction = false; // go "right" from player 0 to 1
  for (int c = 0; c < randomCycles; c++) {
    if( direction ){ // go "left"
      for (int i = 8; i >= 0; i--) {
        displays[1].drawPixel(i, 7, LED_YELLOW);
        displays[1].writeDisplay();
        delay(speed);
        clearTurnIndicator(1);
      }
      for (int i = 8; i >= 0; i--) {
        displays[0].drawPixel(i, 7, LED_YELLOW);
        displays[0].writeDisplay();
        delay(speed);
        clearTurnIndicator(0);
      }
    } else { // go "right"
      for (int i = 0; i < 8; i++) {
        displays[0].drawPixel(i, 7, LED_YELLOW);
        displays[0].writeDisplay();
        delay(speed);
        clearTurnIndicator(0);
      }
      for (int i = 0; i < 8; i++) {
        displays[1].drawPixel(i, 7, LED_YELLOW);
        displays[1].writeDisplay();
        delay(speed);
        clearTurnIndicator(1);
      }
    }
    speed += c;
    direction = !direction;
  }
  displays[direction ? 1 : 0].drawPixel(7, 7, LED_GREEN);
  displays[direction ? 1 : 0].writeDisplay();
  delay(1000);
  doReset();
}

void doReset() {
  setLife(0, INIT_LIFE);
  setLife(1, INIT_LIFE);
  resetButtonCombo[0] = 0;
  resetButtonCombo[1] = 0;
  state = GAME_ON;
}

const int debounceDelay = 200;
void handleButtons() {

  for (int i = 0, l = BUTTON_COUNT; i < l; i++) {
    btn[i] = digitalRead(btnPin[i]);

    if (!btnpressed[i] && btn[i] == HIGH) {
        /* Serial.print("PRESSED "); */
        /* Serial.println(i); */
        btnpressed[i] = true;
        handleButton(i);
    } else if(btnpressed[i] && btn[i] == LOW){
        btnpressed[i] = false;
        delay(debounceDelay);
    }

  }

}

void handleButton(int btnId) {
  switch (btnId) {
    case 0: return setLife(0, lifeTotals[0] + 1);
    case 1: return setLife(0, lifeTotals[0] - 1);
    case 2: return setLife(1, lifeTotals[1] + 1);
    case 3: return setLife(1, lifeTotals[1] - 1);
    case 4: return handleReset(1, 0);
    case 5: return handleReset(0, 1);
  }
}

void display(int displayNum, String text, int color=LED_GREEN) {
  int pos[2] = { text.length() > 1 ? 0 : 2, 5};

  displays[displayNum].setTextColor(color);
  displays[displayNum].clear();
  displays[displayNum].setCursor(pos[0], pos[1]);
  displays[displayNum].print(text);
  displays[displayNum].writeDisplay();
}

int mapLifeToColor(int life) {
  int color = LED_GREEN;
  if( life < 10 ) color = LED_YELLOW;
  if( life < 5 ) color = LED_RED;
  return color;
}

void setLife(int playerNum, int newLife) {
  lifeTotals[playerNum] = newLife;
  display(playerNum, String(newLife), mapLifeToColor(newLife));
}

void clearTurnIndicator(int playerNum) {
  int x = playerNum == 0 ? 0 : 7;
  for (int x = 0; x < 8; x++) {
    displays[playerNum].drawPixel(x, 7, 0);
  }
  displays[playerNum].writeDisplay();
}

const int animationPlayerTurnSpeed = 15; // delay time
void animatePlayerTurn(int opponentNum) {
  int playerNum = opponentNum == 0 ? 1 : 0;

  if( opponentNum == 0 ){
    for (int i = 8; i >= 0; i--) {
      displays[playerNum].drawPixel(i, 7, LED_YELLOW);
      displays[playerNum].writeDisplay();
      delay(animationPlayerTurnSpeed);
      clearTurnIndicator(playerNum);
    }
    for (int i = 8; i >= 0; i--) {
      displays[opponentNum].drawPixel(i, 7, LED_YELLOW);
      displays[opponentNum].writeDisplay();
      delay(animationPlayerTurnSpeed);
      clearTurnIndicator(opponentNum);
    }
    displays[opponentNum].drawPixel(0, 7, LED_RED);
    displays[opponentNum].writeDisplay();
  } else {
    for (int i = 0; i < 8; i++) {
      displays[playerNum].drawPixel(i, 7, LED_YELLOW);
      displays[playerNum].writeDisplay();
      delay(animationPlayerTurnSpeed);
      clearTurnIndicator(playerNum);
    }
    for (int i = 0; i < 8; i++) {
      displays[opponentNum].drawPixel(i, 7, LED_YELLOW);
      displays[opponentNum].writeDisplay();
      delay(animationPlayerTurnSpeed);
      clearTurnIndicator(opponentNum);
    }
    displays[opponentNum].drawPixel(7, 7, LED_RED);
    displays[opponentNum].writeDisplay();
  }
}

bool displayFaces = false;
const int blinkFaceDelay = 500;
void handleVictory() {
  int victor;
  if( lifeTotals[0] <= 0 ) {
    victor = 1;
  } else if( lifeTotals[1] <= 0 ) {
    victor = 0;
  } else return;


  if( displayFaces ){
    for (int p = 0; p < 2; p++) {
      displays[p].clear();
      displays[p].drawBitmap(0, 0, victor == p ? smile_bmp : sad_bmp, 8, 8, victor == p ? LED_GREEN : LED_YELLOW);
      displays[p].writeDisplay();
    }
    delay(blinkFaceDelay);
  } else {
    for (int p = 0; p < 2; p++) {
      display(p, String(lifeTotals[p]), mapLifeToColor(lifeTotals[p]));
    }
    delay(blinkFaceDelay);
  }

  displayFaces = !displayFaces;
}

void handleSonic() {
  for (int i = 0; i < 2; i++) {
    // Clears the trigPin
    digitalWrite(sonicTriggerPins[i], LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(sonicTriggerPins[i], HIGH);
    delayMicroseconds(10);
    digitalWrite(sonicTriggerPins[i], LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    sonicDurations[i] = pulseIn(sonicEchoPins[i], HIGH);
    // Calculating the distance
    sonicDistances[i] = sonicDurations[i]*0.034/2;

    /* Serial.print("Distance "); Serial.print(i); Serial.print(": "); Serial.println(sonicDistances[i]); */
  }

  if(sonicDistances[0] < KNOCK_DISTANCE && currentPlayerTurn == 0) {
    /* clearTurnIndicator(0); */
    Serial.println("Player 1 KNOCKED");
    currentPlayerTurn = 1;
    animatePlayerTurn(currentPlayerTurn);
  }
  if(sonicDistances[1] < KNOCK_DISTANCE && currentPlayerTurn == 1) {
    /* clearTurnIndicator(1); */
    Serial.println("Player 2 KNOCKED");
    currentPlayerTurn = 0;
    animatePlayerTurn(currentPlayerTurn);
  }
}

void loop() {
  if( state == GAME_ON || state == GAME_OVER )
    handleButtons();

  if( state == GAME_ON )
    handleVictory();

  if( state == GAME_ON )
    handleSonic();
}
