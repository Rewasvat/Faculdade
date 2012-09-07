#!/usr/bin/python
import sys
from math import ceil, floor
import random

import scipy
from scipy.stats import uniform, norm, expon, tmean, tstd

####################################################
class DataClass:
    def __init__(self, dist):
        self.dist = dist
        self.mle = None #as return by dist.fit(data); normally is a [loc, scale] array with the distribution estimated parameters
        self.prior = 0.0
        
    def EstimateParameters(self, data):
        # we use our know equations to calculate MLEs for the distributions we use, but
        # we store the values in the same way scipy's MLE calculation function (fit) returns them,
        # so they can be easily passed to the distribution later.
        if self.dist.name == "uniform":
            minD = min(data) # MLE lower bound = min(data)
            maxD = max(data) # MLE upper bound = max(data)
            # uniform value range is [loc, loc+scale]
            self.mle = [floor(minD), ceil(maxD-minD)]
        elif self.dist.name == "norm":
            dataMean = tmean(data) # MLE mean = mean(data)
            dataStd = tstd(data)   # MLE variance = variance(data)
            # loc = mean
            # scale = standard deviation
            self.mle = [dataMean, dataStd]
        elif self.dist.name == "expon":
            # MLE lambda = 1 / mean(data)
            # scale = 1 / lambda
            self.mle = [0, tmean(data)]
        else:
            self.mle = seld.dist.fit(data)
        
    def Likelihood(self, value):
        #likelihood = probabilityDensity (pdf) da distribuicao com os parametros estimados no valor recebido
        return self.dist.pdf(value, loc=self.mle[0], scale=self.mle[1])
        
    def Posterior(self, value, evidence):
        # posterior = p(cI|X)
        # posterior = p(cI) * p(X|cI)    / p(X)
        # posterior = prior * likelihood / evidence
        return self.prior * self.Likelihood(value) / evidence
        

####################################################
class Classifier:
    def __init__(self, c1_dist, c2_dist, c3_dist):
        self.classes = [DataClass(c1_dist), DataClass(c2_dist), DataClass(c3_dist)]
        #self.c1 = DataClass(c1_dist)
        #self.c2 = DataClass(c2_dist)
        #self.c3 = DataClass(c3_dist)
        
    @property
    def c1(self):   return self.classes[0]
    @property
    def c2(self):   return self.classes[1]
    @property
    def c3(self):   return self.classes[2]
    
    def EstimateParameters(self, c1data, c2data, c3data):
        self.c1.EstimateParameters(c1data)
        self.c2.EstimateParameters(c2data)
        self.c3.EstimateParameters(c3data)
        
    def Training(self, c1data, c2data, c3data):
        self.EstimateParameters(c1data, c2data, c3data)
        calculatePrior = lambda data: float(len(data))/(len(c1data)+len(c2data)+len(c3data))
        self.c1.prior = calculatePrior(c1data)
        self.c2.prior = calculatePrior(c2data)
        self.c3.prior = calculatePrior(c3data)
        
    def Evidence(self, value):
        #evidence = p(X)
        #evidence = p(c1)   * p(X|c1)  +  p(c2)   * p(X|c2)  +  p(c3)   * p(X|c3)
        #evidence = (c1.prior)p(X|c1)  +  (c2.prior)p(X|c2)  +  (c3.prior)p(X|c3)
        return self.c1.Posterior(value, 1.0) + self.c2.Posterior(value, 1.0) + self.c3.Posterior(value, 1.0)
        
    def __call__(self, value):  return self.Classify(value)
    def Classify(self, value):
        e = self.Evidence(value)
        posteriors = [c.Posterior(value, e) for c in self.classes]
        p = max(posteriors)
        return posteriors.index(p) + 1
        
    def __repr__(self): return self.__str__()
    def __str__(self):  return "Classifier(%7s/%7s/%7s)" % (self.c1.dist.name, self.c2.dist.name, self.c3.dist.name)
        
####################################################
class ClassifierPool:
    def __init__(self):
        self.dists = [uniform, norm, expon] #use these 3 distributions since they're the ones gerador.py uses.
        self.classifiers = []
        self.selectedClassifier = None
        self.BuildClassifiers()
        
    def BuildClassifiers(self):
        cases = [(d1,d2,d3) for d1 in self.dists for d2 in self.dists for d3 in self.dists]
        for d1, d2, d3 in cases:
            self.classifiers.append( Classifier(d1,d2,d3) )
            
    def RunTraining(self, c1data, c2data, c3data):
        print "Training classifiers..."
        for cfier in self.classifiers:
            cfier.Training(c1data, c2data, c3data)
        print "Training complete."
                
    def RunTests(self, dataPointList):
        print "Running tests..."
        print "\t-Scores are #correctClassifications/#tests. First the total score then the partials for each class."
        print "\t-Classifiers are printed as (Class1 distribution / Class2 distribution / Class3 distribution)"
        best = []       #list of classifiers with the best score (equal values)
        best_score = 0  #score of the best classifier
        totalSamples = len(dataPointList)
        for cfier in self.classifiers:
            score = 0
            classScore = [0, 0, 0]
            classTotals = [0,0,0]
            for dPoint in dataPointList:
                isTrue = (cfier(dPoint.value) == dPoint.classNum)
                score += isTrue
                classScore[dPoint.classNum-1] += isTrue
                classTotals[dPoint.classNum-1] += 1

            print "%s\tscore: %s/%s [C1: %s/%s; C2: %s/%s; C3: %s/%s]" % (cfier, score, totalSamples, classScore[0], classTotals[0], classScore[1], classTotals[1], classScore[2], classTotals[2])
            if score > best_score:
                best = [cfier]
                best_score = score
            elif score == best_score:
                best.append(cfier)
        ###
        print "Selecting best classifier..."
        while len(best) > 1:
            best.pop( random.randint(0, len(best)-1) )
        self.selectedClassifier = best[0]
        print "Selected %s [Correct %2.2f%%]" % (self.selectedClassifier, 100*float(best_score)/totalSamples)
        
####################################################
class DataPoint:
    def __init__(self, value, classNumber):
        self.value = value
        self.classNum = classNumber
        
####################################################
class DataSet:
    def __init__(self, filename):
        self.filename = filename
        self.data = []
        self._readFile()     
        
    def _readFile(self):
        dataFile = open(self.filename, "r")
        for line in dataFile.readlines():
            data = line.split(" ")
            if len(data) != 2:  continue
            value = float(data[0])
            classNumber = int(data[1])
            self.data.append( DataPoint(value, classNumber) )
        dataFile.close()
        
    def GetDataByClasses(self):
        ret = [[], [], []]
        for dataP in self.data:
            ret[dataP.classNum-1].append(dataP.value)
        return ret

        
##################################################################
##################################################################
scipy.seterr(all='ignore')
if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Chamar programa passando o nome dos arquivos com os conjuntos de amostra e de validacao"
        exit()
    classifierPool = ClassifierPool()
    trainingSet = DataSet(sys.argv[1])
    trainingData = trainingSet.GetDataByClasses()
    classifierPool.RunTraining(trainingData[0], trainingData[1], trainingData[2])

    testingSet = DataSet(sys.argv[2])
    classifierPool.RunTests(testingSet.data)


