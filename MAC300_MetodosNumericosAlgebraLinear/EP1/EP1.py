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
import DFT

from numpy import argmax, abs
from numpy.fft import fftfreq

class Note:
    def __init__(self, freq, start, duration, initialAmplitude):
        self.freq = freq
        self.start = start
        self.duration = duration
        self.amplitudeRange = [initialAmplitude]
        
    def AddAmplitudeStep(self, a):
        self.amplitudeRange.append(a)
        
    def GetNormalizedAmplitudes(self):
        m = max(self.amplitudeRange)
        return [a/m for a in self.amplitudeRange]
        
    def __repr__(self): return self.__str__()
    def __str__(self):  return "%2.2fHz at %s secs lasting for %s secs" % (self.freq, self.start, self.duration)
        
        
class Analyzer:
    def __init__(self, data):
        self.frameRate = data[0]
        self.data = data[1]
        self.notes = []
        self.analysisStep = 0.1
        
    def GetBaseDFTBlockSize(self):
        return int(self.frameRate * self.analysisStep)
        
    def analyzeInterval(self, start, length, DFTmethod):
        startIndex = int( start * self.frameRate )
        endIndex = int( startIndex + length*self.frameRate )
        interval = self.data[startIndex:endIndex]
        
        spectrum = DFTmethod(interval)
        i = argmax(abs(spectrum))  # abs(spectrum) gives the amplitude spectrum, argmax returns the index to the maximum value of the array
        freqFactors = fftfreq(len(interval)) #helper function that gives the frequency factors for each position in the array return by the DFT
        
        #normalizedAmplitudes = abs(spectrum) / abs(spectrum)[i]
        #for j in range(len(interval)):
        #    a = normalizedAmplitudes[j]
        #    f = abs(self.frameRate * freqFactors[j])
        #    if a > 0.8:
        #        print "Amplitude: %s (%s Hz)" % (abs(spectrum)[j], f)
                
        return (abs(self.frameRate * freqFactors[i]), abs(spectrum)[i])
    
    def Analyze(self, DFTmethod):
        self.notes = [] #reset our previous analysis, in case there is any.
        duration = len(self.data)/self.frameRate
        print "Analyzing with %s... (frameRate: %sHz :: duration: %2.2f secs)" % (DFTmethod.name, self.frameRate, duration)
        start = 0.0
        note = None
        while start < duration:
            # analyze interval => frequency, amplitude
            f, a = self.analyzeInterval(start, self.analysisStep, DFTmethod)
            if note == None:    note = Note(f, start, self.analysisStep, a)
            else:
                if f != note.freq:
                    # found a different note
                    self.notes.append(note)
                    note = Note(f, start, self.analysisStep, a)
                else:
                    # update note duration and amplitude range
                    note.duration += self.analysisStep
                    note.AddAmplitudeStep(a)
            ##
            start += self.analysisStep
        if note != None:
            self.notes.append(note)
        


########################################################
def ShowSpectogram(analyzer):
    try:
        import pylab
    except:
        print "Warning: module pylab (from matplotlib) could not be found. No charts will be displayed."
        return
    
    print "Producing Wave Time-Domain and Frequency-Domain charts... (close graph window to continue)"
    pylab.subplot(211)
    pylab.plot(analyzer.data)
    pylab.title("Time-Domain")
    
    pylab.subplot(212)
    pylab.specgram(analyzer.data, NFFT=analyzer.GetBaseDFTBlockSize(),
                    Fs=analyzer.frameRate, scale_by_freq=False, sides='default')
    pylab.title("Spectogram (Frequency-Domain)")
    pylab.show()

def Execute(argList):
    if len(argList) <= 0:
        print "Wrong program call. Use: "
        print "EP1.py <file name>.wav"
        return
    argFile = ""
    showGraphs = True
    dftMethod = "both"
    while len(argList) > 0:
        arg = argList.pop()
        if arg == "-nographs":
            showGraphs = False
        elif arg[:5] == "-dft:":
            dftMethod = arg.split(":")[1].lower()
            if not dftMethod in ["matrix", "matrixnumpy", "fft", "both", "bothfast", "all"]:
                print "Invalid DFT Method passed. Defaulting to 'both'"
                dftMethod = "both"
        elif arg.lower().find("wav") != -1:
            argFile = arg
    
    if argFile.lower()[-3:] != "wav":
        print "Wrong program call. A .wav sound file must be passed."
        return
    #
    print "Transcripting \"%s\" to musical notes for MIDI..." % argFile
    wavData = FileUtils.LoadWave(argFile)
    analyzer = Analyzer(wavData)
    
    mDFTs = []
    if dftMethod in ["matrix", "both", "all"]:
        mDFTs.append(  DFT.DFT_Matrix() )
    if dftMethod in ["matrixnumpy", "bothfast", "all"]:
        mDFTs.append(  DFT.DFT_Matrix(True) )
    if dftMethod in ["fft", "both", "bothfast", "all"]:
        mDFTs.append(  DFT.DFT_FFT() )
    for mdft in mDFTs:
        analyzer.Analyze(mdft)
        mdft.PrintResults()
    
    outputName = argFile.split("\\")[-1].split("/")[-1]
    outputName = ".".join(outputName.split(".")[:-1]) + ".MIDI"
    mid = FileUtils.MIDI(outputName)
    for note in analyzer.notes:
        mid.AddNote(note)
    mid.Save()
    print "Saved transcripted MIDI file to \"%s\"!" % outputName
    #
    if showGraphs:
        ShowSpectogram(analyzer)
    #
    return
    
if __name__ == "__main__":
    Execute( sys.argv[1:] )
