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
    return numpy.asarray( imread(name), dtype=numpy.int32 )

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
def RunMultiple(fileName, rankList):
    compressor = Compressor( GetImageFromName() )
    
    
    
def Execute(argList):
    if len(argList) < 2:
        print "Wrong program call. Use: "
        print "EP3.py <compression rank> <file_name>"
        return

    rank = int(argList[0])
    argFile = argList[1]
    
    #
    print "Compressing image \"%s\" with rank k=%s..." % (argFile, rank)
    compressor = Compressor( GetImageFromName() )
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
