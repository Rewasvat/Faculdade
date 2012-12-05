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

def randM(w,h):
    import random
    m = np.asmatrix( np.zeros( (w,h) ))
    for i, j in [(a, b) for a in xrange(w) for b in xrange(h)]:
        m[i,j] = 20 * random.random()
    return m
    
def removeNearZeroentries(M):
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
            if False:#k <= n-2:
                A = A.T
                print "running right"
                x = np.asmatrix(np.copy(A[k+1:m, k]))
                Vk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x))
                Vk = Vk / norm(Vk)
                
                #A[k+1:m, k] = A[k+1:m, k] - 2*(A[k+1:m, k]*Vk)*Vk.T  #check: no algoritmo era 2*(M*Vk)*Vk.T
                A[k+1:m, k:n] = A[k+1:m, k:n] - 2*Vk*(Vk.T*A[k+1:m, k:n])
            
                V[k+1:m, k:n] = V[k+1:m, k:n] - 2*Vk*(Vk.T*V[k+1:m, k:n])
                #V[k:m, k+1:n] = V[k:m, k+1:n] - 2*(V[k:m, k+1:n]*Vk)*Vk.T
                A = A.T
                #continue
                #x = A[k, k+1:n]
                #Vk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x), False)
                #Vk = Vk / norm(Vk)
                #Vk = Vk.T
                #print A
                ##P = matrix householder de Vk  ( P = I - 2v(vT) )
                ## A = A * P
                ## A = A * (I - 2*Vk*Vk.T)
                ## A = A - 2*(A*Vk)*Vk.T
                #A[k:m, k+1:n] = A[k:m, k+1:n] - 2*(A[k:m, k+1:n]*Vk)*Vk.T  #check: no algoritmo era 2*(M*Vk)*Vk.T
                #print A
                ## V = P * V
                ## V = (I - 2*Vk*Vk.T)*V
                ## V = V - 2*Vk*(Vk.T*V)
                ##V[k:m, k+1:n] = V[k:m, k+1:n] - 2*Vk*(Vk.T*V[k:m, k+1:n])
                #V[k:m, k+1:n] = V[k:m, k+1:n] - 2*(V[k:m, k+1:n]*Vk)*Vk.T
                ####
            removeNearZeroentries(A)
            
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
