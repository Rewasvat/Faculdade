# -*- coding: utf-8 -*-

class FilterMethod:
    def __init__(self, name):
        self.results = []
        self.name = name
        self.mark = 0
        
    def __call__(self, data):   return []   #IMPLEMENT THIS in derived class
    
    def markTime(self):
        self.mark = time.clock()
        
    def stop(self):
        t = time.clock() - self.mark
        self.results.append(t)
        
    def GetMeanExecutionTime(self):
        return numpy.mean(self.results)
        
    def GetNumExecutions(self):
        return len(self.results)
        
    def PrintResults(self):
        s = "DFT Method Results for %s: %.5f mean execution time (seconds) for %s calls." % (self.name, self.GetMeanExecutionTime(), self.GetNumExecutions())
        print s
        return s
        
####
class Filter_Contrast(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "FFT")
        
    def __call__(self, data):
        self.markTime()
        spectrum = numpy.fft.fft(data)
        self.stop()
        return spectrum
        
###
class Filter_Blur(FilterMethod):
    def __init__(self, useNumPyMatrix=False):
        name = "Matrix"
        if useNumPyMatrix:
            name += " (NumPy)"
        FilterMethod.__init__(self, name)
        self.matrices = {} #stores computed DFT matrix for each N (N: matrix)
        self.useNumPyMatrix = useNumPyMatrix
        
    def __call__(self, data):
        #create matrix 
        W = self.createMatrix(len(data))
        self.markTime()
        spectrum = self.multiply(data, W)
        self.stop()
        return spectrum

####
class Filter_Sharpen(FilterMethod):
    def __init__(self):
        FilterMethod.__init__(self, "FFT")
        
    def __call__(self, data):
        self.markTime()
        spectrum = numpy.fft.fft(data)
        self.stop()
        return spectrum
