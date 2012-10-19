# -*- coding: utf-8 -*-

import numpy

def roundToNearest(x):
    return int(x+0.5)


class FilterMethod:
    def __init__(self, name):
        self.name = name
        
    def __call__(self, data):   return []   #IMPLEMENT THIS in derived class
    

####
class Filter_CONTRAST(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "CONTRAST")
        self.n = 0
        self.gray_level = [0 for i in range(256)]  #original histogram
        self.cdf = [0 for i in range(256)]
        
    def __call__(self, data):
        self.n = len(data) * len(data[0])
        self.ConstructOriginalHistogram(data)
        self.cdf_min = numpy.min( numpy.trim_zeros(self.cdf) )

        for x in range(len(data)):
            for y in range(len(data[x])):
                data[x,y] = self.NormalizedHistogram( data[x,y] )

        return data

    def ConstructOriginalHistogram(self, data):
        # count occurences of gray level i in image
        for x in range(len(data)):
            for y in range(len(data[x])):
                self.gray_level[ data[x][y] ] += 1

        for i in range(256):
            # normalize count
            self.gray_level[i] = self.gray_level[i] / self.n
            # calculate cdf value
            self.cdf[i] = numpy.sum( self.gray_level[:i+1] )

    def NormalizedHistogram(self, i):
        return roundToNearest( 255 * ( (self.cdf[i] - self.cdf_min) / (self.n - self.cdf_min) ) )


def test():
    from scipy.misc import imshow, lena
    L = lena()
    M = Filter_CONTRAST()
    L = M(L)
    imshow(L)

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
