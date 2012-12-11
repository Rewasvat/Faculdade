# -*- coding: utf-8 -*-

import numpy as np
np.set_printoptions(suppress=True)

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
        transposed = False
        if m < n:  
            transposed = True
            A = A.T  # We need width >= height, so if not we transpose M
        m, n = A.shape
        print m, n
        U = np.asmatrix( np.eye(m, m) )
        V = np.asmatrix( np.eye(n, n) )
        
        for k in xrange(n):
            print "------ K=%s ------" % (k)
            x = np.asmatrix(np.copy(A[k:m, k]))

            Uk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x))
            Uk = Uk / norm(Uk)
            
            Q = np.asmatrix( np.eye(m, m) )
            Q[k:m, k:m] -= 2*Uk*Uk.T
            #Q = matrix householder de Uk ( Q = I - 2v(vT) )
            #A = Q * A            
            A[k:m, k:n] = A[k:m, k:n] - 2*Uk*(Uk.T*A[k:m, k:n])

            #U = U * Q
            # U = U * (I - 2*Uk*Uk.T)
            # U = U - 2*(U*Uk)*Uk.T
            U[0:m, k:m] = U[0:m, k:m] - 2*(U[0:m, k:m]*Uk)*Uk.T
            ###
            if k <= n-2:
                #We transpose A here so that this line-zeroing 'right-householder' 
                #operates 'normally'. A will be transposed back to normal in the end of the IF block
                #A = A.T
                x = np.asmatrix(np.copy(A[k, k+1:n]))
                Vk = x + np.sign(x[0,0])*norm(x)*GetBasisVector(0, len(x))
                Vk = Vk / norm(Vk)
                # P = matrix householder de Vk  ( P = I - 2v(vT) )
                P = np.asmatrix( np.eye(n,n) )
                P[k+1:n, k+1:n] -= 2*Vk.T*Vk
                #A = A * P
                # A = A * (I - 2*Vk*Vk.T)
                # A = A - 2*(A*Vk)*Vk.T
                #A[k+1:n, k:n] = A[k+1:n, k:n] - 2*Vk*(Vk.T*A[k+1:n, k:n])
                A[k:m, k+1:n] = A[k:m, k+1:n] - 2*(A[k:m, k+1:n]*Vk.T)*Vk
                #V = P * V
                # V = (I - 2*Vk*Vk.T)*V
                # V = V - 2*Vk*(Vk.T*V)
                V[k+1:n, 0:n] = V[k+1:n, 0:n] - 2*Vk.T*(Vk*V[k+1:n, 0:n])
                #A = A.T
                
            removeNearZeroEntries(A)
            
        if not transposed:
            return (U, A, V.T)
        else:
            return (U.T, A.T, V)

    
    def executeGolubReinsch(self, U, B, V):
        m, n = B.shape
        eps = 1.0e-12
        count = 1
        while True:
            raw_input()
            print "=================== ITERATION %s ================" % count
            count += 1        
            
            for i in xrange(n-2):
                if np.abs(B[i,i+1]) <= eps*(np.abs(B[i,i]) + np.abs(B[i+1, i+1])):
                    B[i, i+1] = 0
            ###
            q = 0
            for i in xrange(n-1, -1, -1):
                def checkColumn():
                    for j in xrange(m):
                        if B[j,i] != 0 and j!=i:
                            return False
                    return True
                if i > 0 and checkColumn():
                    break
                q += 1
            
            p = n-q
            for i in xrange(n-q-1, -1, -1):
                def checkColumn():
                    for j in xrange(m):
                        if B[j,i] != 0 and j!=i:
                            return False
                    return True
                if i > 0 and checkColumn():#B[i-1, i] == 0:
                    print "fechando p=%s no i=%s" % (p, i)
                    break
                print "atualizando p para %s" % (p)
                p -= 1
            print "----- B -----"
            print B
            print "Q=%s  ::  P=%s" % (q, p)
            #######
            # B tem n colunas, achar o menor p e o maior q tal que
            # elementos nao pertencentes aos blocos sejam 0.
            # B1,1 = p colunas
            # B2,2 = n-p-q colunas; nao tem elementos nulos na superdiagonal (diagonal acima da principal)
            # B3,3 = q colunas; tem que ser diagonal
            #######
            
            ###
            if q == n:
                #S é a diagonal de B
                break
            ###
            for i in xrange(p+1, n-q): # range should be [p+1, n-q-1]  # range(start, stop) => [start, stop[
                # iterando pelas colunas do B2,2
                if B[i,i] == 0:                    
                    #aplicar Givens para B[i,i+1] = 0  e B2,2 ainda eh upper bidiagonal
                    print "AMAGAD"
                    if i == n-q-1: #ultima coluna do B2,2
                        pass
                    else:
                        pass
                else:
                    U, B, V = self.executeGolubKahanSVDstep(n, B, U, V, p, q)
                    #return 0,0,0
        return U, B, V

    def executeGolubKahanSVDstep(self, n, B, U, V, p, q):
        #B2,2 -> bloco diagonal de B, com indices de linha/coluna em [p+1, n-q]
        B2_2 = B[p+1:n-q , p+1:n-q]
        C = B2_2.T * B2_2
        C = C[-2:,-2:]
        eigs = np.linalg.eig(M)[0]
        u = self.getClosestValueTo(eigs, C[1,1])
        k = p+1
        alpha = (B[k,k]**2) - u
        beta = B[k,k] * B[k,k+1]
        for k in xrange(p+1, n-q-1):
            #alpha = (B[k,k]**2) - u ##
            #beta = B[k,k] * B[k,k+1]##
            print "<>--- SVD STEP %s %s---<>" % (k, range(p+1, n-q))
            G = self.getGivensMatrixFor(B.shape, alpha, beta, k, k+1)
            B = B * G.T
            V = V * G.T
            
            print "Alpha=%s   Beta=%s  G:" % (alpha, beta)
            print G
            print B
            
            alpha = B[k,k]
            beta = B[k+1,k]
            G = self.getGivensMatrixFor(B.shape, alpha, beta, k, k+1)
            B = G * B
            U = U * G
            
            print "(2) Alpha=%s   Beta=%s  G:" % (alpha, beta)
            print G
            print B
            
            if k < n-q-1:
                alpha = B[k,k+1]
                beta = B[k,k+2]
        print "------- B in SVD step -----"
        print B
        return U, B, V
        
    def getClosestValueTo(self, l, value):
        ret = l[0]
        for v in l[1:]:
            if np.abs(value-v) < np.abs(value-ret):
                ret = v
        return ret
        
    def getGivensMatrixFor(self, shape, a, b, i, j, r=-1):
        m, n = shape
        G = np.asmatrix( np.zeros( (m, n) ) )
        for x in xrange(min(m,n)):
            G[x,x] = 1

        if r == -1:
            r = np.hypot(a, b)
        else:
            a = b = r / np.sqrt(2)
        cos = a / r
        sin = -b / r
        G[i,i] = G[j,j] = cos
        if i > j:
            G[i,j] = sin
            G[j,i] = -sin
        else:
            G[i,j] = -sin
            G[j,i] = sin
        
        return G
    
    def executeSVD(self, M):
        # Step1: do bidiagonalization
        #U, B, V = self.executeGolubKahan(M)
    
        # Step2: diagonalize
        U, s, V = np.linalg.svd(M)
        return np.asmatrix(U), np.asmatrix(np.diag(s)), np.asmatrix(V)
        
    def CompressMatrix(self, M, rank):
        m, n = M.shape
        transposed = False
        if m < n:
            M = M.T
            transposed = True

        U, S, V = self.executeSVD(M)
        nU = U[:, :rank]
        nS = S[:rank, :rank]
        nV = V[:rank, :]
        out = nU * nS * nV
        if transposed:
            return out.T
        return out
        
    def Compress(self, rank):
        # Run SVD - get U, S and V (of each color submatrix)
        # Cutoff S to be a diagonal matrix of length RANK
        # Fix U and V shape to match size of S (RANKxRANK)
        # Return compressed image = USVt
        if len(self.data.shape) == 3: #imagem com 3 canais de cores - RGB
            R = np.copy(self.data)
            for i in range(3):
                M = self.data[:,:,i]
                R[:,:,i] = self.CompressMatrix(M, rank)
            return R                
        elif len(self.data.shape) == 2: #imagem com um canal de cor (como grayscale, por exemplo)
            return self.CompressMatrix(self.data, rank)
        else:
            print "ERROR: Image format not recognized"
            return M
        
        
        
#########################
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
    
    
def testGivens(i, m=4, n=4):
    M = randM(m,n)
    comp = Compressor(0)
    G = comp.getGivensMatrixFor(M.shape, M[i,i], M[i,i+1], i, i+1)
    
    print "-----G-----"
    print G
    return G
    
    
def svd(m,n):
    matriz = randM(m,n)
    comp = Compressor(0)
    u,b,v = comp.executeGolubKahan(matriz)
    u2, s, v2 = comp.executeGolubReinsch(u,b,v)
    return s
    
