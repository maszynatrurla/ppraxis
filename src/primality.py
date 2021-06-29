#!/usr/bin/env python

from random import randint
from sys import exit

def findRS(n):
    N = n - 1
    for r in xrange(128):
        twoToR = 1 << r
        if twoToR > N:
            break
        if 0 == N % twoToR:
            s = N / twoToR
            if s & 1:
                print "r = ", r
                print "s = ", s
                yield (r,s)

def mypow(a, b, m):
    c = 1
    i = 0
    while True:
        c = (a * c) % m
        i += 1
        if i == b:
            break
    return c

n = 2**89-1

for r,s in findRS(n):
    for i in xrange(50):
        a = randint(1, n - 1)
        print "a = ", a
        if 1 == mypow(a, s, n):#pow(a, s, n):
            continue
        for j in xrange(r):
            if n - 1 == mypow(a, (1 << j) * s, n):#pow(a, (1 << j) * s, n):
                break
        else:
            print "not a prime"
            exit(0)
print "prime"
            
