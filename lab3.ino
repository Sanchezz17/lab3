#include <Arduino.h>
#include "buzzer.h"
#include <MD_TCS230.h>

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 ColorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

#define PIN_BUZZER 10
Buzzer buzzer(PIN_BUZZER);

int notes[] = {NOTE_A5, NOTE_B5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_GS5, NOTE_SILENCE};
double duration = 1;

enum Colors{
  black = 0,
  red = 1,
  green = 2,
  blue = 3,
  purple = 4,
  orange = 5,
  yellow = 6,
  white = 7,
  unknown = 8
};

int currentColor = unknown;
int difference = 5;

void setup() {
    Serial.begin(115200);
    Serial.println("Started!");

    sensorData whiteCalibration;
    whiteCalibration.value[TCS230_RGB_R] = 120060;
    whiteCalibration.value[TCS230_RGB_G] = 117520;
    whiteCalibration.value[TCS230_RGB_B] = 157590;
    sensorData blackCalibration;
    blackCalibration.value[TCS230_RGB_R] = 11280;
    blackCalibration.value[TCS230_RGB_G] = 10270;
    blackCalibration.value[TCS230_RGB_B] = 13230;

    ColorSensor.begin();
    ColorSensor.setDarkCal(&blackCalibration);
    ColorSensor.setWhiteCal(&whiteCalibration);
}

void loop() {
    colorData rgb;
    ColorSensor.read();
    while (!ColorSensor.available());
    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
    currentColor = convert_rgb_to_color(rgb);
    play_color(currentColor); 
}

int convert_rgb_to_color(colorData rgb)
{
    int r = rgb.value[TCS230_RGB_R];
    int g = rgb.value[TCS230_RGB_G];
    int b = rgb.value[TCS230_RGB_B];

    //toDo
}

void play_color(int color)
{
    buzzer.setMelody({notes[color]}, {1}, 1);
    buzzer.turnSoundOn();
    buzzer.playSound();
}

void print_rgb(colorData rgb)
{
    Serial.print(rgb.value[TCS230_RGB_R]);
    Serial.print(" ");
    Serial.print(rgb.value[TCS230_RGB_G]);
    Serial.print(" ");
    Serial.print(rgb.value[TCS230_RGB_B]);
    Serial.println();
}
