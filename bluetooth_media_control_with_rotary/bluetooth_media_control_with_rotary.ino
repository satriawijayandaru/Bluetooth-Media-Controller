#include <BleKeyboard.h>
#include "ClickButton.h"

#define CLK 15
#define DT 14
#define SW 13
#define btn 2
#define led 4

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

BleKeyboard bleKeyboard("Shifter Pro", "Kaname", 69);
ClickButton rotaryBtn(SW, LOW, CLICKBTN_PULLUP);

void setup() {
  Serial.begin(115200);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  //  pinMode(btn, INPUT_PULLUP);
  //  lastStateCLK = digitalRead(CLK);
  rotaryBtn.debounceTime   = 20;
  rotaryBtn.multiclickTime = 400;
  rotaryBtn.longClickTime  = 1000;
  Serial.println("Starting BLE work!"); 
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW); 
  bleKeyboard.begin();
}

void loop() {

  if (bleKeyboard.isConnected()) {
    rotaryBtn.Update();
    rotaryEncoder(1);
  }
}

void rotaryEncoder(int debugEn) {
  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      blinkOnce();
      Serial.println("volume down");
      counter --;
      currentDir = "CCW";

    } else {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      blinkOnce();
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

  if (rotaryBtn.clicks != 0) {
    if (rotaryBtn.clicks == 1) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      blinkOnce();
      Serial.println("1 Click, Play Pause");
    }
    if (rotaryBtn.clicks == 2) {
      bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
      blinkOnce();
      Serial.println("2 Clicks, Next Tract");
    }
    if (rotaryBtn.clicks == 3) {
      bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
      blinkOnce();
      Serial.println("3 Clicks, Previous Track");
    }
  }
  delay(1);
}

void blinkOnce(){
  digitalWrite(led, HIGH);
  delay(25);
  digitalWrite(led, LOW); 
  delay(25);
}
