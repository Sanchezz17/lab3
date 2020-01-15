#include <Arduino.h>
#include "Wire.h"
#include "pitches.h"
#include "colors.h"
#include <Tone.h>

Tone tone1;

#define PIN_BUZZER 6
unsigned long noteStartedMs;

const int address = 10;

int notes[] = {NOTE_A3, NOTE_B3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3, NOTE_GS3, NOTE_SILENCE};

int currentColor = unknown;

void setup()
{    
    tone1.begin(PIN_BUZZER);
    pinMode(PIN_BUZZER, OUTPUT);
    Serial.begin(115200);
    Wire.begin();
}

void loop() 
{
    currentColor = readColor();
    Serial.print("Current color: ");
    Serial.println(currentColor);
    tone1.play(notes[currentColor]);
    delay(2000);
}

int readColor()
{
    Wire.requestFrom(address, 1);
    if (Wire.available()) 
      return Wire.read();
}
