#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "midi_note.h"

#define MIDI_CHANNEL 15;

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

/* setup midi */

void setupMidi()
{
  return;
}

void setup() {
  /* Start Serial */
  Serial.begin(11502000);
  
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

float getPeakFreq()
{
/* gets the peak frequency of the speach */
/* might want to put a HPF or a band pass filter to get rid of the low frequency stuff */

return 0.0;
}


float freq_out = 440.0;

float getDesFreq()
{
  char note;
  char channel;
  int is_midi = 0;
  if(usbMIDI.read())
  {
    is_midi = processMIDI(&channel, &note);
  }

  if (is_midi == 1)
  {
    if ( channel == MIDI_CHANNEL )
    {
      if ( note < NOTE_MAX )
      {
        freq_out = NOTES[note];
      }
      
    }
  }
  /* MOCK: get desired frequency */
  return freq_out;
}

void loop() {
  // put your main code here, to run repeatedly:

  /* get the peak frequency */
  float f_mes = getPeakFreq();

  /* gains for the PID controller for the peak finder */
  float kp_freq = 1.0;
  float ki_freq = 0.0;
  float kd_freq = 0.0;

  /* Desired Frequency - From midi */
  float f_des = getDesFreq();

  /* Error between desired and actuial freq */
  float e_freq = f_des - f_mes;

  /* Control output from PID, need to change it to a ratio (i.e. if no error it is equal to 1) */
  float f_ctrl = kp_freq * e_freq;

  /* ratio of the error of the frequency to the actuial frequency... */
  /* wait... can I just do the raito of the measured and actuial frequency? */
  float f_ratio = f_des / f_mes;

  /* TODO: set it up so you can adjust the speed of the change, this should built into the granular library */

  granular1.setSpeed(f_ratio);

  /* Continuously adjust speed based on the A2 Pot */
  /* A2D converter */
  /*
  float knobA2 = (float)analogRead(A2) / 1023.0;
  float ratio = 0.0;
  ratio = powf( 2.0, knobA2 * 2.0 - 1.0 );
  granular1.setSpeed(ratio);
  */

}


int processMIDI(byte * ret_channel, byte * ret_note) {
  byte type, channel, data1, data2, cable;

  // fetch the MIDI message, defined by these 5 numbers (except SysEX)
  //
  type = usbMIDI.getType();       // which MIDI message, 128-255
  channel = usbMIDI.getChannel(); // which MIDI channel, 1-16
  data1 = usbMIDI.getData1();     // first data byte of message, 0-127
  data2 = usbMIDI.getData2();     // second data byte of message, 0-127
  cable = usbMIDI.getCable();     // which virtual cable with MIDIx8, 0-7

  // uncomment if using multiple virtual cables
  //Serial.print("cable ");
  //Serial.print(cable, DEC);
  //Serial.print(": ");

  // print info about the message
  //
  switch (type) {
    case usbMIDI.NoteOff: // 0x80
      Serial.print("Note Off, ch=");
      Serial.print(channel, DEC);
      Serial.print(", note=");
      Serial.print(data1, DEC);
      Serial.print(", velocity=");
      Serial.println(data2, DEC);
      break;

    case usbMIDI.NoteOn: // 0x90
      Serial.print("Note On, ch=");
      Serial.print(channel, DEC);
      Serial.print(", note=");
      Serial.print(data1, DEC);
      Serial.print(", velocity=");
      Serial.println(data2, DEC);
      *ret_channel = channel;
      *ret_note = data1;
      return 1;
      break;

    case usbMIDI.AfterTouchPoly: // 0xA0
      Serial.print("AfterTouch Change, ch=");
      Serial.print(channel, DEC);
      Serial.print(", note=");
      Serial.print(data1, DEC);
      Serial.print(", velocity=");
      Serial.println(data2, DEC);
      break;

    case usbMIDI.ControlChange: // 0xB0
      Serial.print("Control Change, ch=");
      Serial.print(channel, DEC);
      Serial.print(", control=");
      Serial.print(data1, DEC);
      Serial.print(", value=");
      Serial.println(data2, DEC);
      break;

    case usbMIDI.ProgramChange: // 0xC0
      Serial.print("Program Change, ch=");
      Serial.print(channel, DEC);
      Serial.print(", program=");
      Serial.println(data1, DEC);
      break;

    case usbMIDI.AfterTouchChannel: // 0xD0
      Serial.print("After Touch, ch=");
      Serial.print(channel, DEC);
      Serial.print(", pressure=");
      Serial.println(data1, DEC);
      break;

    case usbMIDI.PitchBend: // 0xE0
      Serial.print("Pitch Change, ch=");
      Serial.print(channel, DEC);
      Serial.print(", pitch=");
      Serial.println(data1 + data2 * 128, DEC);
      break;

    case usbMIDI.SystemExclusive: // 0xF0
      // Messages larger than usbMIDI's internal buffer are truncated.
      // To receive large messages, you *must* use the 3-input function
      // handler.  See InputFunctionsComplete for details.
      Serial.print("SysEx Message: ");
      printBytes(usbMIDI.getSysExArray(), data1 + data2 * 256);
      Serial.println();
      break;

    case usbMIDI.TimeCodeQuarterFrame: // 0xF1
      Serial.print("TimeCode, index=");
      Serial.print(data1 >> 4, DEC);
      Serial.print(", digit=");
      Serial.println(data1 & 15, DEC);
      break;

    case usbMIDI.SongPosition: // 0xF2
      Serial.print("Song Position, beat=");
      Serial.println(data1 + data2 * 128);
      break;

    case usbMIDI.SongSelect: // 0xF3
      Serial.print("Sond Select, song=");
      Serial.println(data1, DEC);
      break;

    case usbMIDI.TuneRequest: // 0xF6
      Serial.println("Tune Request");
      break;

    case usbMIDI.Clock: // 0xF8
      Serial.println("Clock");
      break;

    case usbMIDI.Start: // 0xFA
      Serial.println("Start");
      break;

    case usbMIDI.Continue: // 0xFB
      Serial.println("Continue");
      break;

    case usbMIDI.Stop: // 0xFC
      Serial.println("Stop");
      break;

    case usbMIDI.ActiveSensing: // 0xFE
      Serial.println("Actvice Sensing");
      break;

    case usbMIDI.SystemReset: // 0xFF
      Serial.println("System Reset");
      break;

    default:
      Serial.println("Opps, an unknown MIDI message type!");
      
    return 0;
  }
}


void printBytes(const byte *data, unsigned int size) {
  while (size > 0) {
    byte b = *data++;
    if (b < 16) Serial.print('0');
    Serial.print(b, HEX);
    if (size > 1) Serial.print(' ');
    size = size - 1;
  }
}
