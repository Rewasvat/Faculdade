# -*- coding: utf-8 -*-

import numpy as np

def norm(v):
    value = 0
    #print v
    #print v.shape
    #print value
    for i, j in [(a, b) for a in xrange(v.shape[0]) for b in xrange(v.shape[1])]:
    #for i in xrange(len(v)):
        value += v[i,j]**2
        #print value
    return np.sqrt(value)
    
def GetBasisVector(index, size, columnVector=True):
    v = np.zeros( (size) )
    v[index] = 1
    if columnVector:
        return np.asmatrix(v).T
    else:
        return np.asmatrix(v)
    
def removeNearZeroEntries(M):
    for i, j in [(a, b) for a in xrange(M.shape[0]) for b in xrange(M.shape[1])]:
        if np.abs(M[i,j]) < 1.0e-12:
            M[i,j] = 0
    
    
class Compressor:
    def __init__(self, imageData):
        self.data = imageData
        
    def executeGolubKahan(self, M):
        A = np.asmatrix(np.copy(M))
        
        m, n = A.shape
        if m < n:  A = A.T  # We need width >= height, so if not we transpose M
        m, n = A.shape
        #U = np.asmatrix( np.zeros( (m, m) ) )
        #V = np.asmatrix( np.zeros( (n, n) ) )
        ##U = np.asmatrix( np.identity(m) )
        U = np.asmatrix( np.zeros( (m, n) ) )
        for i in xrange(min(m,n)):
            U[i,i] = 1
        V = np.asmatrix( np.identity(n) )
        
        for k in xrange(n):
            print "------ K=%s ------" % (k)
            x = np.asmatrix(np.copy(A[k:m, k]))

            Uk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x))
            Uk = Uk / norm(Uk)
            #Q = matrix householder de Uk ( Q = I - 2v(vT) )
            # A = Q * A            
            A[k:m, k:n] = A[k:m, k:n] - 2*Uk*(Uk.T*A[k:m, k:n])

            # U = U * Q
            # U = U * (I - 2*Uk*Uk.T)
            # U = U - 2*(U*Uk)*Uk.T
            U[0:m, k:n] = U[0:m, k:n] - 2*(U[0:m, k:n]*Uk)*Uk.T
            ###
            if k <= n-2:
                #We transpose A here so that this line-zeroing 'right-householder' 
                #operates 'normally'. A will be transposed back to normal in the end of the IF block
                A = A.T
                x = np.asmatrix(np.copy(A[k+1:m, k]))
                Vk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x))
                Vk = Vk / norm(Vk)
                # P = matrix householder de Vk  ( P = I - 2v(vT) )
                # A = A * P
                # A = A * (I - 2*Vk*Vk.T)
                # A = A - 2*(A*Vk)*Vk.T
                A[k+1:m, k:n] = A[k+1:m, k:n] - 2*Vk*(Vk.T*A[k+1:m, k:n])
                # V = P * V
                # V = (I - 2*Vk*Vk.T)*V
                # V = V - 2*Vk*(Vk.T*V)
                V[k+1:m, 0:n] = V[k+1:m, 0:n] - 2*Vk*(Vk.T*V[k+1:m, 0:n])
                A = A.T
                
            removeNearZeroEntries(A)
            
        return (U, A, V.T)
        
    def executeGolubReinsch(self, U, B, V):
        pass

    def executeSVD(self, M):
        # Step1: do bidiagonalization
        U, B, V = executeGolubKahan(M)
        
        # Step2: diagonalize
        
    def Compress(self, rank):
        # Run SVD - get U, S and V
        # Cutoff S to be a diagonal matrix of length RANK
        # Fix U and V shape to match size of S (RANKxRANK)
        # Return compressed image = USVt
        pass
        
        
        
########################3
# DEBUG STUFF - REMOVE IT

def randM(w,h):
    import random
    m = np.asmatrix( np.zeros( (w,h) ))
    for i, j in [(a, b) for a in xrange(w) for b in xrange(h)]:
        m[i,j] = 20 * random.random()
    return m
    
def t(mmm,nnn):
    matriz = randM(mmm,nnn)
    comp = Compressor(0)
    u,b,v = comp.executeGolubKahan(matriz)
    return matriz,u,b,v
    
def t2(mmmm,nnnn):
    m,u,b,v = t(mmmm,nnnn)
    print "------ M ------"
    print m
    print "------ UBVt ------"
    print u*b*v.T
    print "--- expected V.T ---"
    #m = u*b*v.T
    #u.I*m = b*v.T
    #b.I*u.I*m = v.T
    evT = b.I * u.I * m
    removeNearZeroEntries(evT)
    print evT
    print "-------- V.T ------"
    print v.T
    return str(m) == str(u*b*v.T)
