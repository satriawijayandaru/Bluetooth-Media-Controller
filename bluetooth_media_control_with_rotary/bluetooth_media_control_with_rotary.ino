#include <BleKeyboard.h>

#define CLK 15
#define DT 14
#define SW 13
#define btn 2

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

BleKeyboard bleKeyboard;

void setup() {
  Serial.begin(115200);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(btn, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
  Serial.println("Starting BLE work!");

  bleKeyboard.begin();
}

void loop() {
//  Serial.println(digitalRead(btn));
//  delay(200);
  if (bleKeyboard.isConnected()) {
    if(digitalRead(btn) == 0){
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    }
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
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
    if (millis() - lastButtonPress > 50) {
      if (debugEn == 1) {
        bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        Serial.println("Button pressed!");
      }
    }
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(1);
}
