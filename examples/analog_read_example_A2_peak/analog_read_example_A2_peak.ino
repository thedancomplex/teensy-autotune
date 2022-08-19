#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputAnalog         adc1;           //xy=192,87
AudioAnalyzeRMS          rms1;           //xy=392,175
AudioAmplifier           amp1;           //xy=443,96
AudioAnalyzePeak         peak1;          //xy=613,104
AudioConnection          patchCord1(adc1, amp1);
AudioConnection          patchCord2(adc1, rms1);
AudioConnection          patchCord3(amp1, peak1);
// GUItool: end automatically generated code



void setup() {
  // put your setup code here, to run once:
  AudioMemory(60);
  amp1.gain(1.0);
  pinMode(A2, INPUT);
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

  if(rms1.available())
  {
    Serial.println(rms1.read());
  }
  if(peak1.available())
  {
    Serial.println(peak1.read());
  }
  else
  {
    //Serial.println('.');
  }
  //Serial.println(peak1.read());
  delay(10);
}
