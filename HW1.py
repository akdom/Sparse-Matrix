import math
def T(n):
     if n == 2:
         return 1
     else:
         return math.sqrt(n)*T(math.sqrt(n))+n

def build_n(k):
    return 2**2**k

def prod(x,y):
    return x * y

def solution(k):
    return reduce(prod, map(build_n, xrange(1,k-1))) + reduce(sum, map(build_n, xrange(1,k)))
