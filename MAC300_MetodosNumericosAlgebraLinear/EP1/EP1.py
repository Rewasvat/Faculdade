#!/usr/bin/python
# -*- coding: utf-8 -*-

########################################################
#
#  MAC300 - EP1
#  Fernando Omar Aluani  #USP: 6797226
#
#   Esse é o principal arquivo do EP, o "main"
#   Rode-o para executar o programa.
#
########################################################
import sys

def Finish():
    if sys.platform == "win32":
        # workaround para que ao rodar o script diretamente no Windows
        # e uma janela de console seja aberta (ou não), ela não feche
        # assim que a execução terminar.
        print "\nPress ENTER to close..."
        raw_input()
    exit()

if __name__ == "__main__":
    if len(sys.argv) <= 1:
        print "Wrong program call. Use: "
        print "EP1.py <file name>.wav"
        Finish()
    arg = sys.argv[1]
    if arg.lower()[-3:] != "wav":
        print "Wrong program call. File passed must be .wav sound file."
        Finish()
    #
    Finish()
