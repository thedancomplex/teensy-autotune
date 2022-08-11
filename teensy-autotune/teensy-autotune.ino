#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=233,238
AudioMixer4              mixer1;         //xy=454,238
AudioEffectGranular      granular1;      //xy=692,239
AudioAmplifier           amp1;           //xy=902,239
AudioOutputI2S           i2s2;           //xy=1108,239
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 2);
AudioConnection          patchCord3(i2s1, 1, mixer1, 1);
AudioConnection          patchCord4(i2s1, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, granular1);
AudioConnection          patchCord6(granular1, amp1);
AudioConnection          patchCord7(amp1, 0, i2s2, 0);
AudioConnection          patchCord8(amp1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=142,135
// GUItool: end automatically generated code


/* Uncomment if want to enable SD Card */
/*
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14
*/

void setup() {
  // put your setup code here, to run once:
  /* Set Audio Memory */
  AudioMemory(64);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  /* Uncomment if want to enable SD Card */
  /*
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    // stop here, but print a message repetitively
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  */
  

}

void loop() {
  // put your main code here, to run repeatedly:

}
