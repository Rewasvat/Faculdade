#!/usr/bin/python
# -*- coding: utf-8 -*-

########################################################
#
#  MAC300 - EP2
#  Fernando Omar Aluani  #USP: 6797226
#
#   Esse é o principal arquivo do EP, o "main"
#   Rode-o para executar o programa.
#
########################################################
import sys
import Filters
import FileUtils

        
class ImageProcessor:
    def __init__(self, imagedata):
        self.data = imagedata
        
    def GetBaseDFTBlockSize(self):
        return int(self.frameRate * self.analysisStep)

    def analyzeInterval(self, start, length, DFTmethod):
        startIndex = int( start * self.frameRate )
        endIndex = int( startIndex + length*self.frameRate )
        baseInterval = self.data[startIndex:endIndex]
        size = len(baseInterval)
        
        interval = hanning(size) * baseInterval

        spectrum = DFTmethod(interval)
        amplitudes = abs(spectrum)
        i = argmax(amplitudes)  # abs(spectrum) gives the amplitude spectrum, argmax returns the index to the maximum value of the array
        freqFactors = fftfreq(size) #helper function that gives the frequency factors for each position in the array return by the DFT

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
def Execute(argList):
    if len(argList) <= 2:
        print "Wrong program call. Use: "
        print "EP2.py <filter_method> <file_name>"
        return

    filterMethod = argList[0][1:]
    #TODO: check if method is valid
    argFile = argList[1]

    #
    print "Processing image \"%s\" with %s filter..." % (argFile, filterMethod.upper())
    imageData = FileUtils.LoadImage(argFile)
    processor = ImageProcessor(imageData)
    
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
