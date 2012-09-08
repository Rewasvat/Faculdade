# -*- coding: utf-8 -*-

import wave, struct
import math
from midiutil.MidiFile import MIDIFile

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
    for i in range(0,length):
        frameData = waveFile.readframes(1)
        sampleData = struct.unpack(formatstr, frameData)
        data.append(sampleData[0]) #since it's mono, there'll be only one value
        
    return (waveFile.getframerate(), data)
    
    
def roundToNearest(x):
    return int(x+0.5)

def GetMIDIcode(freq):
    #calculating basing from note A4 - 440 Hz
    return roundToNearest(69 + 12*math.log(freq/440.0, 2))

# simple wrapper around MIDIFile to more easily suit our purposes
import midiutil.smidi
class MIDI:
    def __init__(self, filename):
        self.filename = filename
        self.midi = midiutil.smidi.MidiOutFile(filename)
        self.midi.header()
        self.midi.start_of_track()
        self.beatsPerMinute = 120
        self.midi.tempo(roundToNearest(60000000.0/self.beatsPerMinute))
        self.midi.patch_change(0, 79)
        self.midi.continuous_controller(0, midiutil.smidi.SOUND_CONTROLLER_3, 127) #sound release time
        
    def AddNote(self, note):
        # note should be from class Note
        # we receive start & duration in seconds
        if note.freq == 0.0:    return
        pitch = GetMIDIcode(note.freq)
        print "Adding note (MIDI Code: %s) %s" % (pitch, note)
        startBeats = 95*note.start * (self.beatsPerMinute/60.0)
        durationBeats = 95*note.duration * (self.beatsPerMinute/60.0)
        self.midi.update_time(roundToNearest(startBeats), False)
        #self.midi.continuous_controller(0, midiutil.smidi.SUSTAIN_ONOFF, 127) # value < 64 -> OFF // value > 64 -> ON
        self.midi.note_on(note=pitch, velocity=127)
        self.midi.update_time(roundToNearest(startBeats + durationBeats), False)
        #self.midi.continuous_controller(0, midiutil.smidi.SUSTAIN_ONOFF, 0)
        self.midi.note_off(note=pitch, velocity=127)
        
    def Save(self):
        self.midi.update_time(0)
        self.midi.end_of_track()
        self.midi.eof()
        
###
class MIDIXXX:
    def __init__(self, filename):
        self.filename = filename
        self.midi = MIDIFile(1)
        self.midi.addTrackName(0,0,"MusicTranscription")
        self.beatsPerMinute = 120
        self.midi.addTempo(0, 0, self.beatsPerMinute) #tempo, beats per minute
        
    def AddNote(self, note):
        # note should be from class Note
        # we receive start & duration in seconds
        # MIDIFile expects start & duration in beats
        if note.freq == 0.0:    return
        print "Adding note", note
        pitch = GetMIDIcode(note.freq)
        startBeats = note.start * (self.beatsPerMinute/60.0)
        durationBeats = note.duration * (self.beatsPerMinute/60.0)
        volume = 120   #volume/velocity of the note (in [0-127])
        self.midi.addNote(0,0, pitch, startBeats, durationBeats, volume)
        
    def Save(self):
        midiFile = open(self.filename, 'wb')
        self.midi.writeFile(midiFile)
        midiFile.close()


########################3
#REMOVE
def teste(filedata, starting_sec, length):
    frameRate = float(filedata[0])
    data = filedata[1]
    duration = len(data)/frameRate
    
    dataStartingIndex = int(starting_sec*frameRate)
    dataEndIndex = int(dataStartingIndex + length*frameRate)
    data = data[dataStartingIndex:dataEndIndex]
    
    freqs = fft(data)
    i = argmax(abs(freqs))
    freqFactors = fftfreq(len(data))
    #return frameRate * i / len(data)
    return abs(frameRate * freqFactors[i])
    
    
def teste2(filedata, start, step, duration=-1):
    if duration==-1:
        duration = len(filedata[1])/filedata[0]
    while start < duration:
        print "freq=%s [%s->%s]" % (teste(filedata,start,step), start, start+step)
        start += step
    
import os, sys
if sys.platform == "win32":
    sampledir = ".\\files\\samples\\"
else:
    sampledir = "./files/samples/"
files = [sampledir+nome for nome in os.listdir(sampledir)]

def run(i, step=0.25):
	fd = LoadWave(files[i])
	teste2(fd,0,step)
	print files[i]

