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

from numpy import argmax, abs, mean, concatenate, zeros
from numpy.fft import fftfreq

###
def decimate(x, q, n=None, ftype='iir', axis=-1):
    """downsample the signal x by an integer factor q, using an order n filter
    By default, an order 8 Chebyshev type I filter is used or a 30 point FIR
    filter with hamming window if ftype is 'fir'.

    (port to python of the GNU Octave function decimate.)

    Inputs:
    x -- the signal to be downsampled (N-dimensional array)
    q -- the downsampling factor
    n -- order of the filter (1 less than the length of the filter for a 'fir' filter)
    ftype -- type of the filter; can be 'iir' or 'fir'
    axis -- the axis along which the filter should be applied

    Outputs:
    y -- the downsampled signal
    """
    from scipy.signal import cheby1, firwin, lfilter
    if type(q) != type(1):
        raise Error, "q should be an integer"

    if n is None:
        if ftype == 'fir':
            n = 30
        else:
            n = 8

    if ftype == 'fir':
        b = firwin(n+1, 1./q, window='hamming')
        y = lfilter(b, 1., x, axis=axis)
    else:
        (b, a) = cheby1(n, 0.05, 0.8/q)
        y = lfilter(b, a, x, axis=axis)
    return y.swapaxes(0,axis)[::q].swapaxes(0,axis)
###

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
        self.duration = len(self.data)/self.frameRate
        self.notes = []
        self.analysisStep = 0.1
        
    def GetBaseDFTBlockSize(self):
        return int(self.frameRate * self.analysisStep)

    def analyzeInterval(self, start, length, DFTmethod):
        startIndex = int( start * self.frameRate )
        endIndex = int( startIndex + length*self.frameRate )
        intervalWAT = self.data[startIndex:endIndex]
        
        from numpy import blackman, hamming, hanning
        interval = hanning(len(intervalWAT)) * intervalWAT
        size = len(interval)

        spectrum = DFTmethod(interval)
        amplitudes = abs(spectrum)
        i = argmax(amplitudes)  # abs(spectrum) gives the amplitude spectrum, argmax returns the index to the maximum value of the array
        freqFactors = fftfreq(size) #helper function that gives the frequency factors for each position in the array return by the DFT
        
        normalizedAmplitudes = amplitudes / amplitudes[i]
        #croppedAmps = [amplitudes[j]*(normalizedAmplitudes[j]>0.001) for j in xrange(len(interval))]
        meanAmp = mean(amplitudes)
        #print "Mean Amplitude = %s" % (meanAmp)
        croppedAmps = [amplitudes[j]*(amplitudes[j]>meanAmp) for j in xrange(size)]
        prev = 0
        for j in range(size):
            a = croppedAmps[j]
            f40 = abs(self.frameRate * freqFactors[j])
            fMIDI = FileUtils.GetMIDIcode(f40)
            #if f40 == 43.0:
            #    print "Amplitude: %s (%s Hz)(MIDI: %s) ====" % (amplitudes[j], f40, FileUtils.GetMIDIcode(f40))
            if a > croppedAmps[prev]:
                prev = j
            elif a < croppedAmps[prev]:
                f = abs(self.frameRate * freqFactors[prev])
                if f == 0.0:    continue
                #print "Amplitude: %s (%s Hz)(MIDI: %s) DOWN" % (amplitudes[prev], f, FileUtils.GetMIDIcode(f))
                break
            elif a == 0 and croppedAmps[prev] > 0:
                #we found the first peak - prev is the index of maximum value
                f = abs(self.frameRate * freqFactors[prev])
                #print "Amplitude: %s (%s Hz)(MIDI: %s)" % (amplitudes[prev], f, FileUtils.GetMIDIcode(f))
                break
        #i = prev

        return (abs(self.frameRate * freqFactors[i]), amplitudes[i])
    
    def Analyze(self, DFTmethod):
        self.notes = [] #reset our previous analysis, in case there is any.
        print "Analyzing with %s... (frameRate: %sHz :: duration: %2.2f secs)" % (DFTmethod.name, self.frameRate, self.duration)
        start = 0.0
        note = None
        while start < self.duration:
            # analyze interval => frequency, amplitude
            f, a = self.analyzeInterval(start, self.analysisStep, DFTmethod)
            if note == None:    note = Note(f, start, self.analysisStep, a)
            else:
                if f != note.freq:
                    # found a different note
                    self.notes.append(note)
                    #print "NOTE (MIDI Code: %s) %s" % (FileUtils.GetMIDIcode(note.freq), note)
                    note = Note(f, start, self.analysisStep, a)
                else:
                    # update note duration and amplitude range
                    note.duration += self.analysisStep
                    note.AddAmplitudeStep(a)
            ##
            start += self.analysisStep
        #print "NOTE (MIDI Code: %s) %s" % (FileUtils.GetMIDIcode(note.freq), note)
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
    size = len(analyzer.data)
    timeRangeData = [1.0*analyzer.duration*i/size for i in xrange(size)]
    pylab.plot( timeRangeData, analyzer.data)
    pylab.title("Time-Domain")
    #pylab.xlabel("time")
    #pylab.ylabel("dunno")
    
    pylab.subplot(212)
    pylab.specgram(analyzer.data, NFFT=analyzer.GetBaseDFTBlockSize(),
                    Fs=analyzer.frameRate, scale_by_freq=False, sides='default')
    #pylab.ylim( -10, 5000)
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

###
def testall(dftm="fft"):
    for s in FileUtils.files:
        Execute([s, "-dft:"+dftm, "-nographs"])
        print "========================================================="
