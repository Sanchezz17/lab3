#include <Arduino.h>
#include "Wire.h"
#include <MD_TCS230.h>

#define  S0_OUT  2
#define  S1_OUT  3
#define  S2_OUT  4
#define  S3_OUT  5
MD_TCS230 colorSensor(S2_OUT, S3_OUT, S0_OUT, S1_OUT);

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

int radius = 10;
int redRGB[] = {101, 16, 17};
int greenRGB[] ={16, 40, 20};
int blueRGB[] ={19, 67, 98};
int purpleRGB[] = {20, 19, 43};
int orangeRGB[] = {125, 30, 17};
int yellowRGB[] = {142, 99, 30};

int currentColor = unknown;

const int address = 10;

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

    colorSensor.begin();
    colorSensor.setDarkCal(&blackCalibration);
    colorSensor.setWhiteCal(&whiteCalibration);

    Wire.begin(address);
    Wire.onRequest(handleRequest);
}

void loop() {
    colorData rgb;
    colorSensor.read();
    while (!colorSensor.available());
    colorSensor.getRGB(&rgb);
    print_rgb(rgb);
    currentColor = convert_rgb_to_color(rgb);
    Serial.println(currentColor);
}

int convert_rgb_to_color(colorData rgb)
{
    int r = rgb.value[TCS230_RGB_R];
    int g = rgb.value[TCS230_RGB_G];
    int b = rgb.value[TCS230_RGB_B];

    if (r < radius && g < radius && b < radius)
      return black;
    if (255 - r < radius && 255 - g < radius && 255 - b < radius)
      return white;
    if (abs(redRGB[0]- r) < radius && abs(redRGB[1] - g) < radius && abs(redRGB[2] - b) < radius)
      return red;
    if (abs(purpleRGB[0]- r) < radius && abs(purpleRGB[1] - g) < radius && abs(purpleRGB[2] - b) < radius)
      return purple;
    if (abs(yellowRGB[0]- r) < radius && abs(yellowRGB[1] - g) < radius && abs(yellowRGB[2] - b) < radius)
      return yellow;
    if (abs(orangeRGB[0]- r) < radius && abs(orangeRGB[1] - g) < radius && abs(orangeRGB[2] - b) < radius)
      return orange;
    if (abs(greenRGB[0]- r) < radius && abs(greenRGB[1] - g) < radius && abs(greenRGB[2] - b) < radius)
      return green;
    if (abs(blueRGB[0]- r) < radius && abs(blueRGB[1] - g) < radius && abs(blueRGB[2] - b) < radius)
      return blue;
    return unknown;
}

void print_rgb(colorData rgb)
{
    Serial.print(rgb.value[TCS230_RGB_R]);
    Serial.print(" ");
    Serial.print(rgb.value[TCS230_RGB_G]);
    Serial.print(" ");
    Serial.println(rgb.value[TCS230_RGB_B]);
}

void handleRequest() {
    Wire.write(currentColor);
}
