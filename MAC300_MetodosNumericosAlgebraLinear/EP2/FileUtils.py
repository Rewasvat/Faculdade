# -*- coding: utf-8 -*-

from scipy.misc import imread, imsave, lena

def LoadImage(name):
    return imread(name, flatten=1)

def SaveImage(name, image):
    imsave(name, image):

def GetTestImage():
    return lena()
