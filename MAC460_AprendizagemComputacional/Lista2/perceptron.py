#!/usr/bin/python

import sys
from numpy import ndarray, asarray, linalg, dot

def extend_samples(lc1, lc2):
    ec1 = []
    for v in lc1:
        v.insert(0, 1)
        ec1.append(v)
    ec2 = []
    for v in lc2:
        v.insert(0, 1)
        v = [-i for i in v]
        ec2.append(v)
    return asarray( ec1 + ec2 )

def sum_wrong_samples(Y, a):
    ret = asarray([0,0,0])
    for v in Y:
        if dot(v, a) <= 0:
            ret += v
    return ret
    

def perceptron_batch( Y, n, theta):
    a = asarray( [0,0,0] )
    k = 0
    delta = asarray( [theta*2, theta*2, theta*2] )
    while linalg.norm(delta) >= theta:
        k = k+1
        delta = n * sum_wrong_samples(Y, a)
        print "Iteration %s: delta = %s(length=%2.4f) [delta/n=%s]" % (k, delta, linalg.norm(delta), linalg.norm(delta)/n  )
        a = a + delta
    print "Finished in %s iterations" % k
    return a


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print "Execute the script passing in the value for 'n (eta)' and the value for 'theta', in this order."
    else:
        x1 = [0, 0]
        x2 = [0, 1]
        x3 = [1, 0]
        x4 = [1, 1]
        extY = extend_samples( [x1,x2], [x3,x4] )
        print "Y = %s" % ( extY )
        N = float(sys.argv[1])
        T = float(sys.argv[2])
        print "Params: n=%s  theta=%s" % (N,T)
        print "A = %s" % ( perceptron_batch( extY, N, T) )
