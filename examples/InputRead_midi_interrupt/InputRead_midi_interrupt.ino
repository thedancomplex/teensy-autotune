/* Receive Incoming USB MIDI by reading data.  This approach
   gives you access to incoming MIDI message data, but requires
   more work to use that data.  For the simpler function-based
   approach, see InputFunctionsBasic and InputFunctionsComplete.

   Use the Arduino Serial Monitor to view the messages
   as Teensy receives them by USB MIDI

   You must select MIDI from the "Tools > USB Type" menu

   This example code is in the public domain.
*/

#include "midi_note.h"
#include "midi_functions.h"

/* add interrupt */
IntervalTimer midiTimer;

#define MIDI_CHANNEL 15

int   m_midi_note = 60;
float m_midi_freq = 440.0;

void doMidi()
{
  char note    = 0;
  char channel = 0;
  int  is_midi = 0;
  
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
        m_midi_freq = NOTES[note];
        m_midi_note = note;
      }
      
    }
  }
  return;
}


int midiSetup()
{
  midiTimer.priority(145);
  midiTimer.begin(doMidi, 10);
}

void setup() {
  Serial.begin(115200);
  midiSetup();
}

void loop() {
  // usbMIDI.read() needs to be called rapidly from loop().  When
  // each MIDI messages arrives, it return true.  The message must
  // be fully processed before usbMIDI.read() is called again.
  Serial.println(m_midi_note);
  delay(100);
}
