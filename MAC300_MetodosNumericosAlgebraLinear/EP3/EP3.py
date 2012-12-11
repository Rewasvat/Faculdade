#!/usr/bin/python
# -*- coding: utf-8 -*-

########################################################
#
#  MAC300 - EP3
#  Fernando Omar Aluani  #USP: 6797226
#
#   Esse é o principal arquivo do EP, o "main"
#   Rode-o para executar o programa.
#
########################################################
import sys
import Compressor

import numpy
from scipy.misc import imread, imsave, lena

def LoadImage(name):
    return numpy.asarray( imread(name), dtype=numpy.float64 )

def SaveImage(name, image):
    imsave(name, image)

def GetTestImage():
    return lena()
    
def GetImageFromName(fileName):
    if fileName == "TEST":
        return GetTestImage()
    else:
        return LoadImage(fileName)


########################################################
def RunMultiple(fileName):
    compressor = Compressor.Compressor( GetImageFromName(fileName) )
    try:
        import pylab
    except:
        print "ERROR: Pylab not found."
        return
    def PlotImage(image, title):
        pylab.imshow(image/255.0, cmap=pylab.cm.gray) #cmap=pylab.cm.gray
        pylab.title(title)
        
    pylab.subplot(231)
    PlotImage(compressor.data, "Original Image")
    
    rankList = [256, 128, 64, 32, 16]
    for i in xrange(len(rankList)):
        k = rankList[i]
        pylab.subplot(232+i)
        img = compressor.Compress(k)
        PlotImage(img, "Compressed Image, k=%s"%(k) )
    
    #pylab.subplot(233)
    #img128 = compressor.Compress(128)
    #PlotImage(img128, "Processed Image")
    
    #pylab.subplot(234)
    #img64 = compressor.Compress(64)
    #PlotImage(img64, "Processed Histogram")
    
    #pylab.subplot(235)
    #img32 = compressor.Compress(32)
    #PlotImage(img32, "Processed Histogram")
    
    #pylab.subplot(236)
    #img16 = compressor.Compress(16)
    #PlotImage(img16, "Processed Histogram")
    
    #show plots
    pylab.show()
    
    
    
def Execute(argList):
    if len(argList) < 2:
        print "Wrong program call. Use: "
        print "EP3.py <compression rank> <file_name>"
        return

    rank = int(argList[0])
    argFile = argList[1]
    
    #
    print "Compressing image \"%s\" with rank k=%s..." % (argFile, rank)
    compressor = Compressor.Compressor( GetImageFromName(argFile) )
    newImageData = compressor.Compress(rank)
    
    outputName = argFile.split("\\")[-1].split("/")[-1]
    outputName = outputName.split(".")[:-1]
    if len(outputName) == 0:    outputName.append(argFile)
    outputName = ".".join(outputName) + "-compressed.bmp"
    SaveImage(outputName, newImageData)
    print "Saved compressed image file to \"%s\"!" % outputName
    return
    
if __name__ == "__main__":
    Execute( sys.argv[1:] )
