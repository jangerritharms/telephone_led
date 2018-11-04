#include "FastLED.h"

#define NUM_LEDS 50

CRGB leds[NUM_LEDS];

int needToPrint = 0;
int count;
int in = 2;
int lastState = LOW;
int trueState = LOW;
long lastStateChangeTime = 0;
int cleared = 0;
int val = 0;

int dialHasFinishedRotatingAfterMs = 100;
int debounceDelay = 10;

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
//  pinMode(6, INPUT);
  // put your setup code here, to run once:
//  Serial.begin(115200);
  pinMode(in, INPUT);
}

void activate_all(CRGB color) {
  for (int i=0;i < NUM_LEDS; i++) {
    leds[i] = color;
  }
  FastLED.show();
}

void up_down(CRGB color) {
  for (int i=0;i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(50);
  }
  for (int i=NUM_LEDS-1;i >= 0; i--) {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(50);
  }
}

void loop() {
//  val = digitalRead(6);
//  Serial.print(val);
  
  // put your main code here, to run repeatedly:
  int reading = digitalRead(in);

  if ((millis() - lastStateChangeTime) > dialHasFinishedRotatingAfterMs) {
    // the dial isn't being dialed, or has just finished being dialed.
    if (needToPrint) {
      // if it's only just finished being dialed, we need to send the number down the serial
      // line and reset the count. We mod the count by 10 because '0' will send 10 pulses.
      Serial.print(count % 10, DEC);
      if (count % 10 == 0) {
        activate_all(CRGB::Black);
      }
      else if (count % 10 == 1) {
        activate_all(CRGB::White);
      }
      else if (count % 10 == 2) {
        up_down(CRGB::Green);
      }
      else if (count % 10 == 3) {
        activate_all(CRGB::Pink);
      }
      else if (count % 10 == 4) {
        activate_all(CRGB::Blue);
      }
      else if (count % 10 == 5) {
        activate_all(CRGB::Green);
      }
      else if (count % 10 == 6) {
        activate_all(CRGB::Yellow);
      }
      else if (count % 10 == 7) {
        activate_all(CRGB::Orange);
      }
      else if (count % 10 == 8) {
        activate_all(CRGB::Green);
      }
      else if (count % 10 == 9) {
        activate_all(CRGB::Green);
      }
      else {
        activate_all(CRGB::Red);
      }
      needToPrint = 0;
      count = 0;
      cleared = 0;
    }
  }

  if (reading != lastState) {
    lastStateChangeTime = millis();
  }

  if ((millis() - lastStateChangeTime) > debounceDelay) {
    // debounce - this happens once it's stablized
    if (reading != trueState) {
      // this means that the switch has either just gone from closed->open or vice versa.
      trueState = reading;
      if (trueState == HIGH) {
        // increment the count of pulses if it's gone high.
        count++; 
        needToPrint = 1; // we'll need to print this number (once the dial has finished rotating)
      } 
    }
  }
  
  lastState = reading;
}
