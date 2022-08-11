#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=233,238
AudioMixer4              mixer1;         //xy=454,238
AudioEffectGranular      granular1;      //xy=692,239
AudioAnalyzePeak         peak1;          //xy=705,423
AudioAnalyzeToneDetect   tone1;          //xy=706,477
AudioAnalyzeFFT256       fft256_1;       //xy=709,373
AudioAnalyzeFFT1024      fft1024_1;      //xy=712,322
AudioAnalyzeNoteFrequency notefreq1;      //xy=716,531
AudioAmplifier           amp1;           //xy=902,239
AudioOutputI2S           i2s2;           //xy=1104,238
AudioConnection          patchCord1(i2s1, 0, mixer1, 0);
AudioConnection          patchCord2(i2s1, 0, mixer1, 2);
AudioConnection          patchCord3(i2s1, 1, mixer1, 1);
AudioConnection          patchCord4(i2s1, 1, mixer1, 3);
AudioConnection          patchCord5(mixer1, granular1);
AudioConnection          patchCord6(mixer1, fft1024_1);
AudioConnection          patchCord7(mixer1, fft256_1);
AudioConnection          patchCord8(mixer1, peak1);
AudioConnection          patchCord9(mixer1, tone1);
AudioConnection          patchCord10(mixer1, notefreq1);
AudioConnection          patchCord11(granular1, amp1);
AudioConnection          patchCord12(amp1, 0, i2s2, 0);
AudioConnection          patchCord13(amp1, 0, i2s2, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=142,135
// GUItool: end automatically generated code



/* Set memory for for granular */
#define GRANULAR_MEMORY_SIZE 12800  // enough for 290 ms at 44.1 kHz
int16_t granularMemory[GRANULAR_MEMORY_SIZE];


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

  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.5);
  mixer1.gain(2, 0.0);
  mixer1.gain(3, 0.0);

  /* the Granular effect requires memory to operate */
  granular1.begin(granularMemory, GRANULAR_MEMORY_SIZE);
  

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

  /* Continuously adjust speed based on the A2 Pot */
  /* A2D converter */
  float knobA2 = (float)analogRead(A2) / 1023.0;
  float ratio = 0.0;
  ratio = powf( 2.0, knobA2 * 2.0 - 1.0 );
  granular1.setSpeed(ratio);

}
