# -*- coding: utf-8 -*-


class Compressor:
    def __init__(self, imageData):
        self.data = imageData
        
    def executeSVD(self, M):
        # Step1: do bidiagonalization
        width, height = M.shape
        if height > width:  M = M.H  # We need width >= height, so if not we transpose M
        for k in xrange(height):
            x = M[k:width, k]
            
        
        # Step2: diagonalize
        return
        
    def Compress(self, rank):
        # Run SVD - get U, S and V
        # Cutoff S to be a diagonal matrix of length RANK
        # Fix U and V shape to match size of S (RANKxRANK)
        # Return compressed image = USVt
