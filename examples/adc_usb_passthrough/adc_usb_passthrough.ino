#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=192,87
AudioAmplifier           amp1;           //xy=443,96
AudioEffectRectifier     rectify1;       //xy=570,170
AudioOutputUSB           usb1;           //xy=706,125
AudioConnection          patchCord1(adc1, amp1);
AudioConnection          patchCord2(amp1, 0, usb1, 0);
AudioConnection          patchCord3(amp1, rectify1);
AudioConnection          patchCord4(rectify1, 0, usb1, 1);
// GUItool: end automatically generated code


void setup() {
  // put your setup code here, to run once:
  AudioMemory(64);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);

}
