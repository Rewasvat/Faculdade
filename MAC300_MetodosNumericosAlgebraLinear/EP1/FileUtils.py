# -*- coding: utf-8 -*-

import wave, struct

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
    
from numpy import argmax
from numpy.fft import fft, fftfreq
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


import math

def roundToNearest(x):
    return int(x+0.5)

def GetMIDIcode(freq):
    #calculating basing of note A4 - 440 Hz
    return roundToNearest(69 + 12*math.log(freq/440.0, 2))

