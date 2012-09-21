# -*- coding: utf-8 -*-
import time
import numpy

class DFTMethod:
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
        print "DFT Method Results for %s: %.5f mean execution time (seconds) for %s calls." % (self.name, self.GetMeanExecutionTime(), self.GetNumExecutions())
        
####
class DFT_FFT(DFTMethod):
    def __init__(self):
        DFTMethod.__init__(self, "FFT")
        
    def __call__(self, data):
        self.markTime()
        spectrum = numpy.fft.fft(data)
        self.stop()
        return spectrum
        
###
class DFT_Matrix(DFTMethod):
    def __init__(self, useNumPyMatrix=False):
        name = "Matrix"
        if useNumPyMatrix:
            name += " (NumPy)"
        DFTMethod.__init__(self, name)
        self.matrices = {} #stores computed DFT matrix for each N (N: matrix)
        self.useNumPyMatrix = useNumPyMatrix
        
    def __call__(self, data):
        #create matrix 
        W = self.createMatrix(len(data))
        self.markTime()
        spectrum = self.multiply(data, W)
        self.stop()
        return spectrum
            
    #returns V * M
    def multiply(self, V, M): 
        ret = []
        size = len(V)
        if self.useNumPyMatrix:
            X = V * M
        for i in xrange(size):
            if self.useNumPyMatrix:
                ret.append( X[0,i] )
            else:
                ret.append( numpy.dot(M[i], V) )
        return ret
    
    def createMatrix(self, N):
        # we use numpy's complex value type to maintain an 'equivalence' to numpy.fft.fft return value.
        # As for the mathematical constant pi and the functions sqrt and exp, I see no difference in using
        # the ones provided by numpy or the ones from standard Python. I chose to use numpy's in this case
        # since we're already using it anyway...
        
        #check if matrix already exists, and return her
        if self.matrices.has_key(N):    return self.matrices[N]
        
        Mdata = []
        if self.useNumPyMatrix:
            Mdata = numpy.matrix(numpy.zeros((N,N)), numpy.complex128)
        #sqrtN = numpy.sqrt(N)
        w = numpy.exp( numpy.complex128(-2j*numpy.pi)/N )  # w= e ^(-2(pi)i/N)
        for i in xrange(N):
            row = []
            start = 0
            if self.useNumPyMatrix: start = i
            for j in xrange(start,N):
                Mij = ( w**(i*j) ) #matrix[i,j] = ( w^(i*j) )/sqrtN
                # we did not divide by sqrt(N) like the definition/convention does because
                # apparently numpy.fft.fft does not as well...
                if not self.useNumPyMatrix:
                    row.append( Mij )
                else:
                    Mdata[i,j] = Mij
                    Mdata[j,i] = Mij
            if not self.useNumPyMatrix:
                Mdata.append( numpy.asarray(row) )
        self.matrices[N] = Mdata
        return Mdata
        
        
##################
def compareComplex(c1, c2, precision=5):
	r1 = c1.real
	i1 = c1.imag
	r2 = c2.real
	i2 = c2.imag
	okR = round(r1, precision) == round(r2, precision)
	okI = round(i1, precision) == round(i2, precision)
	return okR and okI
