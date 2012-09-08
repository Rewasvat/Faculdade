#!/usr/bin/python
# -*- coding: utf-8 -*-

########################################################
#
#  MAC300 - EP1
#  Fernando Omar Aluani  #USP: 6797226
#
#   Esse é o principal arquivo do EP, o "main"
#   Rode-o para executar o programa.
#
########################################################
import sys
import FileUtils

from numpy import argmax
from numpy.fft import fft, fftfreq

class Note:
    def __init__(self, freq, start, duration):
        self.freq = freq
        self.start = start
        self.duration = duration
        
    def __repr__(self): return self.__str__()
    def __str__(self):  return "%2.2fHz at %s secs lasting for %s secs" % (self.freq, self.start, self.duration)
        
class Analyzer:
    def __init__(self, data):
        self.frameRate = data[0]
        self.data = data[1]
        self.notes = []
        self.analysisStep = 0.2
        
    def analyzeInterval(self, start, length, DFTmethod=fft):
        startIndex = int( start * self.frameRate )
        endIndex = int( startIndex + length*self.frameRate )
        interval = self.data[startIndex:endIndex]
        
        spectrum = DFTmethod(interval)
        i = argmax(abs(spectrum))  # abs(spectrum) gives the amplitude spectrum, argmax returns the index to the maximum value of the array
        freqFactors = fftfreq(len(interval)) #helper function that gives the frequency factors for each position in the array return by the DFT
        
        return abs(self.frameRate * freqFactors[i])
    
    def Analyze(self, DFTmethod=fft):
        duration = len(self.data)/self.frameRate
        print "Analyzing... (frameRate: %s :: duration: %2.2f)" % (self.frameRate, duration)
        start = 0.0
        note = None
        while start < duration:
            # analyze interval
            f = self.analyzeInterval(start, self.analysisStep, DFTmethod)
            if note == None:    note = Note(f, start, self.analysisStep)
            else:
                if f != note.freq:
                    # found a different note
                    self.notes.append(note)
                    note = Note(f, start, self.analysisStep)
                else:
                    # update note duration
                    note.duration += self.analysisStep
            ##
            start += self.analysisStep
        if note != None:
            self.notes.append(note)
        


########################################################
def Execute(argList):
    if len(argList) <= 0:
        print "Wrong program call. Use: "
        print "EP1.py <file name>.wav"
        return
    arg = argList[0]
    if arg.lower()[-3:] != "wav":
        print "Wrong program call. File passed must be .wav sound file."
        return
    #
    print "Transcripting \"%s\" to musical notes for MIDI..." % arg
    wavData = FileUtils.LoadWave(arg)
    analyzer = Analyzer(wavData)
    analyzer.Analyze()
    
    outputName = arg.split("\\")[-1].split("/")[-1]
    outputName = ".".join(outputName.split(".")[:-1]) + ".MIDI"
    mid = FileUtils.MIDI(outputName)
    for note in analyzer.notes:
        mid.AddNote(note)
    mid.Save()
    print "Saved transcripted MIDI file to \"%s\"!" % outputName
    #
    return
    
if __name__ == "__main__":
    Execute( sys.argv[1:] )
