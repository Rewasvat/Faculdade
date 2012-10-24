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

import numpy
from scipy.misc import imread, imsave, lena

def LoadImage(name):
    return numpy.asarray( imread(name, flatten=1), dtype=numpy.int32 )

def SaveImage(name, image):
    imsave(name, image)

def GetTestImage():
    return lena()


########################################################
def Execute(argList):
    if len(argList) < 2:
        print "Wrong program call. Use: "
        print "EP2.py <filter_method> <file_name> [-compare]"
        return

    filterMethod = argList[0][1:].upper()
    existingFilters = [fm[7:] for fm in dir(Filters) if fm[:7]=="Filter_"]
    if not filterMethod in existingFilters:
        print "Unrecognized filter name \'%s\'. Possible filters are: %s" % (filterMethod, ", ".join(existingFilters) )
        return

    argFile = argList[1]
    
    showComparison = False
    if len(argList) >= 3:
        showComparison = argList[2].lower() == "-compare"

    #
    print "Processing image \"%s\" with %s filter..." % (argFile, filterMethod)
    if argFile == "TEST":
        imageData = GetTestImage()
    else:
        imageData = LoadImage(argFile)

    method = getattr(Filters, "Filter_"+filterMethod )()
    
    newImageData = method(imageData)
    if showComparison:
        print "Showing image comparison..."
        method.ShowComparison(imageData, newImageData)
    
    outputName = argFile.split("\\")[-1].split("/")[-1]
    outputName = outputName.split(".")[:-1]
    if len(outputName) == 0:    outputName.append(argFile)
    outputName = ".".join(outputName) + "-final.jpg"
    SaveImage(outputName, newImageData)
    print "Saved processed image file to \"%s\"!" % outputName
    return
    
if __name__ == "__main__":
    Execute( sys.argv[1:] )
