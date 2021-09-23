#include <BleKeyboard.h>
#include "ClickButton.h"

#define CLK 15
#define DT 14
#define SW 13
#define btn 2

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

BleKeyboard bleKeyboard("Shifter Pro", "Kaname", 69);
ClickButton button1(SW, LOW, CLICKBTN_PULLUP);

void setup() {
  Serial.begin(115200);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  //  pinMode(btn, INPUT_PULLUP);
  //  lastStateCLK = digitalRead(CLK);
  button1.debounceTime   = 20;
  button1.multiclickTime = 400;
  button1.longClickTime  = 1000;
  Serial.println("Starting BLE work!");

  bleKeyboard.begin();
}

void loop() {

  if (bleKeyboard.isConnected()) {
    button1.Update();
    rotaryEncoder(1);
  }
}

void rotaryEncoder(int debugEn) {
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      Serial.println("volume down");
      counter --;
      currentDir = "CCW";

    } else {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      Serial.println("volume up");
      counter ++;
      currentDir = "CW";
    }
    if (debugEn == 1) {
      Serial.print("Direction: ");
      Serial.print(currentDir);
      Serial.print(" | Counter: ");
      Serial.println(counter);
    }
  }
  lastStateCLK = currentStateCLK;

  if (button1.clicks != 0) {
    if (button1.clicks == 1) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      Serial.println("1 Click, Play Pause");
    }
    if (button1.clicks == 2) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      Serial.println("2 Clicks, Next Tract");
    }
    if (button1.clicks == 3) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
      Serial.println("3 Clicks, Previous Track");
    }
  }
  delay(1);
}
