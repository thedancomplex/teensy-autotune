#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=245,102
AudioSynthWaveformSine   sine1;          //xy=276,177
AudioAmplifier           amp1;           //xy=443,96
AudioAmplifier           amp2;           //xy=476,164
AudioOutputUSB           usb1;           //xy=706,125
AudioConnection          patchCord1(adc1, amp1);
AudioConnection          patchCord2(sine1, amp2);
AudioConnection          patchCord3(amp1, 0, usb1, 0);
AudioConnection          patchCord4(amp2, 0, usb1, 1);
// GUItool: end automatically generated code


void setup() {
  // put your setup code here, to run once:
  AudioMemory(64);
  amp1.gain(0.5);
  amp2.gain(0.5);
  sine1.amplitude(0.5);
  sine1.frequency(440.0);
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(10);

}
