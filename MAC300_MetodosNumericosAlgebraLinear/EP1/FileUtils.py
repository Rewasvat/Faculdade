# -*- coding: utf-8 -*-

import wave, struct
import math
import MIDILib.smidi

def LoadWave(filename):
    waveFile = wave.open(filename, 'r')
    
    #we assume the wav file is mono
    sample_width = waveFile.getsampwidth()
    formatstr = "<"  #format string used to unpack bytes from the WAV. '<' says it's little-endian (like all WAVs)
    if sample_width == 1:
        formatstr += "b"
    elif sample_width == 2:
        formatstr += "h"
    elif sample_width == 4:
        formatstr += "i"
    elif sample_width == 8:
        formatstr += "q"
    else:
        raise Exception("Unrecognized sample width")
        
    data = []
    length = waveFile.getnframes()
    for i in xrange(0,length):
        frameData = waveFile.readframes(1)
        sampleData = struct.unpack(formatstr, frameData)
        data.append(sampleData[0]) #since it's mono, there'll be only one value
        
    return (waveFile.getframerate(), data)
    
def roundToNearest(x):
    return int(x+0.5)

def GetMIDIcode(freq):
    #calculating basing from note A4 - 440 Hz
    if freq == 0.0: return -1
    return roundToNearest(69 + 12*math.log(freq/440.0, 2))

def GetEquivalentValueInRange(origin_value, origin_range, destination_range):
    xa = origin_value - origin_range[0]                 # xa = X - A
    ba = origin_range[1] - origin_range[0]              # ba = B - A
    if ba == 0.0:
        ba = 0.0001
    dc = destination_range[1] - destination_range[0]    # dc = D - C
    r = dc * xa / ba                                    # r = (D-C)(X-A)/(B-A)
    return destination_range[0] + r                     # return C + r
    
# simple wrapper around MIDIFile to more easily suit our purposes
class MIDI:
    def __init__(self, filename):
        self.filename = filename
        self.midi = MIDILib.smidi.MidiOutFile(filename)
        self.midi.header()
        self.midi.start_of_track()
        self.beatsPerMinute = 120
        self.midi.tempo(roundToNearest(60000000.0/self.beatsPerMinute))
        #Changing instrument to 79 (Ocarina) - I think it more correctly matches the original sounds
        #that were made available for examples.
        self.midi.patch_change(0, 79)
        self.midi.continuous_controller(0, MIDILib.smidi.SOUND_CONTROLLER_3, 127) #sound release time
        
    def AddNote(self, note):
        # note should be from class Note
        # we receive start & duration in seconds
        if note.freq == 0.0:    return
        pitch = GetMIDIcode(note.freq)
        print "Adding note (MIDI Code: %s) %s" % (pitch, note)
        startBeats = 95*note.start * (self.beatsPerMinute/60.0)
        durationBeats = 95*note.duration * (self.beatsPerMinute/60.0)
        amps = note.GetNormalizedAmplitudes() #we will use the amplitudes to variate the note volume
        stepBeats = 95*(note.duration/len(amps)) *(self.beatsPerMinute/60.0)
        
        self.midi.update_time(roundToNearest(startBeats), False)
        self.midi.note_on(note=pitch, velocity=127)
        
        prevVol = 0
        for i in range(len(amps)):
            # this is kinda ugly, but ensures that the volume transitions smoothly between
            # one point in amps to the next.
            midvol = roundToNearest(127*amps[i])
            
            if i == len(amps)-1:
                nextvol = 0
            else:
                nextvol = roundToNearest(127*(amps[i] + amps[i+1])/2.0)
            
            pbv = 0
            for beatNum in xrange(roundToNearest(stepBeats)):
                if beatNum < stepBeats/2:
                    beatvol = GetEquivalentValueInRange(beatNum, [0, stepBeats/2], [prevVol, midvol])
                elif beatNum == stepBeats/2:
                    beatvol = midvol
                else:
                    beatvol = GetEquivalentValueInRange(beatNum, [stepBeats/2, stepBeats], [midvol, nextvol])
                #
                if pbv == beatvol:  continue
                pbv = beatvol
                self.midi.continuous_controller(0, 0x07, beatvol) # 0x07 => code for Main Volume (coarse)
                self.midi.update_time(roundToNearest(startBeats + i*stepBeats + beatNum), False)
            prevVol = nextvol

        self.midi.update_time(roundToNearest(startBeats + durationBeats), False)
        self.midi.note_off(note=pitch, velocity=127)
        
    def Save(self):
        self.midi.update_time(0)
        self.midi.end_of_track()
        self.midi.eof()