# -*- coding: utf-8 -*-

import numpy
import pylab

def roundToNearest(x):
    return int(x+0.5)


class FilterMethod:
    def __init__(self, name):
        self.name = name
        
    def __call__(self, data):   return []   #IMPLEMENT THIS in derived class
    
    def GetConvolutedPixel(self, x, y, data, mask):
        pixel = 0
        S, T = mask.shape
        W, H = data.shape
        for s in range(-(S/2), 1 + S/2):
            for t in range(-(T/2), 1 + T/2):
                i = x + s
                j = y + t
                if i < 0 or j < 0 or i >= W or j >= H:  continue
                pixel += mask[s,t] * data[i,j]
        return pixel
        
    def ConvoluteImage(self, data, mask):
        new_data = numpy.zeros(data.shape, data.dtype)
        width, height = data.shape
        for x in range(width):
            for y in range(height):
                new_data[x,y] = self.GetConvolutedPixel(x, y, data, mask)
        return new_data
    
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
        width, height = data.shape
        self.n = width * height
        self.ConstructOriginalHistogram(data)
        self.cdf_min = numpy.min( numpy.trim_zeros(self.cdf) )

        new_data = numpy.zeros(data.shape, data.dtype)
        for x in range(width):
            for y in range(height):
                new_data[x,y] = self.NormalizedHistogram( data[x,y] )
                self.new_gray_level[ new_data[x,y] ] += 1.0

        return new_data

    def ConstructOriginalHistogram(self, data):
        # count occurences of gray level i in image
        width, height = data.shape
        for x in range(width):
            for y in range(height):
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
        w = numpy.asmatrix( [ [1.0/16, 2.0/16, 1.0/16],
                              [2.0/16, 4.0/16, 2.0/16],
                              [1.0/16, 2.0/16, 1.0/16] ])
        new_data = self.ConvoluteImage(data, w)
        return new_data

####
class Filter_SHARPEN(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "SHARPEN")
        
    def __call__(self, data):
        new_data = numpy.zeros(data.shape, data.dtype)
        
        return new_data