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
        
    def Process(self, method):
        pass
        


########################################################
def Execute(argList):
    if len(argList) <= 2:
        print "Wrong program call. Use: "
        print "EP2.py <filter_method> <file_name>"
        return

    filterMethod = argList[0][1:]
    existingFilters = [fm[7:] for fm in dir(Filters) if fm[:7]=="Filter_"]
    if not filterMethod in existingFilters:
        print "Unrecognized filter name \'%s\'. Possible filters are: %s" % (filterMethod, ", ".join(existingFilters) )
        return

    argFile = argList[1]

    #
    print "Processing image \"%s\" with %s filter..." % (argFile, filterMethod.upper())
    imageData = FileUtils.LoadImage(argFile)
    processor = ImageProcessor(imageData)
    
    method = getattr(Filters, "Filter_"+filterMethod.upper() )
    processor.Process(method)
    
    outputName = argFile.split("\\")[-1].split("/")[-1]
    outputName = ".".join(outputName.split(".")[:-1]) + ".jpg"
    FileUtils.SaveImage(outputName, processor.data)
    print "Saved processed image file to \"%s\"!" % outputName
    return
    
if __name__ == "__main__":
    Execute( sys.argv[1:] )
