# -*- coding: utf-8 -*-

import numpy
import pylab

def roundToNearest(x):
    return int(x+0.5)


class FilterMethod:
    def __init__(self, name):
        self.name = name
        
    def __call__(self, data):   return []   #IMPLEMENT THIS in derived class
    
    def PlotImage(self, image, title):
        pylab.imshow(image, cmap=pylab.cm.gray)
        pylab.title(title)
        
    def ShowComparison(self, image, procImage):
        #show original image
        pylab.subplot(121)
        self.PlotImage(image, "Original Image")
        
        #show processed image
        pylab.subplot(122)
        self.PlotImage(procImage, "Processed Image")

        #show plots
        pylab.show()
    

####
class Filter_CONTRAST(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "CONTRAST")
        self.n = 0
        self.gray_level = [0 for i in range(256)]  #original histogram
        self.cdf = [0 for i in range(256)]
        self.new_gray_level = [0 for i in range(256)] #processed histogram
        
    def __call__(self, data):
        self.n = len(data) * len(data[0])
        self.ConstructOriginalHistogram(data)
        self.cdf_min = numpy.min( numpy.trim_zeros(self.cdf) )

        new_data = numpy.zeros(data.shape, data.dtype)
        for x in range(len(data)):
            for y in range(len(data[x])):
                new_data[x,y] = self.NormalizedHistogram( data[x,y] )
                self.new_gray_level[ new_data[x,y] ] += 1.0

        return new_data

    def ConstructOriginalHistogram(self, data):
        # count occurences of gray level i in image
        for x in range(len(data)):
            for y in range(len(data[x])):
                self.gray_level[ data[x,y] ] += 1.0

        for i in range(256):
            # normalize count
            #self.gray_level[i] = self.gray_level[i] / self.n
            # calculate cdf value
            self.cdf[i] = numpy.sum( self.gray_level[:i+1] )

    def NormalizedHistogram(self, i):
        return roundToNearest( 255 * ( (self.cdf[i] - self.cdf_min) / (self.n - self.cdf_min) ) )

    def PlotImageHistogram(self, values, title):
        pylab.bar(numpy.arange(256), values, width=0.8, linewidth=0, color='b')
        pylab.xlabel("Gray Intensity")
        pylab.xlim(0, 255)
        pylab.ylabel("Occurrences in Image")
        pylab.title(title)
    
    def ShowComparison(self, image, procImage):
        #show original image
        pylab.subplot(221)
        self.PlotImage(image, "Original Image")
        
        #show original histogram
        pylab.subplot(222)
        self.PlotImageHistogram(self.gray_level, "Original Histogram")
        
        #show processed image
        pylab.subplot(223)
        self.PlotImage(procImage, "Processed Image")
        
        #show processed histogram
        pylab.subplot(224)
        self.PlotImageHistogram(self.new_gray_level, "Processed Histogram")
        
        #show plots
        pylab.show()

###
class Filter_BLUR(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "BLUR")
        
    def __call__(self, data):
        pass

####
class Filter_SHARPEN(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "SHARPEN")
        
    def __call__(self, data):
        pass
