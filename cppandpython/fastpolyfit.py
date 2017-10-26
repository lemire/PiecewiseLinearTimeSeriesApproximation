# In the future, might switch to scipy
#
# http://prdownloads.sourceforge.net/numpy/scipy_core-0.6.1.tar.gz
# from scipy import * #this is needed for some of the matrix stuff
# from scipy.basic.linalg import *  # part of scipy

# Instead, download the Numeric package... 
try :
  from Numeric import *
  from LinearAlgebra import *
except ImportError:
  print "Install and download the Numeric package"
  print " http://sourceforge.net/project/showfiles.php?group_id=1369&package_id=1351"


###########################################################################
###########################################################################
## the next section of the code offers a way to precompute some 
## polynomial range queries in order to be able to compute in O(1) time
## the fit error using a polynomial of degree d.
##
## Given some data points and the desired polynomial degree
##, you compute a buffer (see computeBuffer)
## in time O(n). You do this once.
## Then you call squaredfiterror to compute the error in time O(1).
## the squaredfiterror function can compute the squared fit error
## over many ranges of the data points.
###########################################################################
###########################################################################

def computeBuffer(datapoints, degree):
  """ this precomputes the range sums so that we can
  best fit a polynomial in constant time! The trick is
  to use prefix sums (has been known for some time). 
  There exists even smarter methods,
  see my CASCON 2002 paper."""
  fastsumbuffer = zeros([2*degree+1,3,len(datapoints)+1],'d')
  for xexponent in xrange(2*degree+1): 
    # we compute more than we need, but what the heck!
    for yexponent in xrange(3):
      fastsumbuffer[xexponent][yexponent][0] = 0.0#datapoints[0][0]**xexponent * datapoints[0][1]**yexponent
      for x in xrange(len(datapoints)):
        fastsumbuffer[xexponent][yexponent][x+1] = fastsumbuffer[xexponent][yexponent][x] +  datapoints[x][0]**xexponent * datapoints[x][1]**yexponent
  return fastsumbuffer

def fastsum(fastsumbuffer,start,end,xexponent, yexponent):
  """ this computes any range sum in constant time """
  try :
    return fastsumbuffer[xexponent][yexponent][end]- fastsumbuffer[xexponent][yexponent][start]
  except IndexError:
    if(xexponent >= len(fastsumbuffer)):
      print "xexponent is too large: ", xexponent, " you can go up to ", len(fastsumbuffer)
      raise
    if(yexponent >= len(fastsumbuffer[xexponent])):
      print "yexponent is too large: ", yexponent, " you can go up to ", len(fastsumbuffer[xexponent])
      raise
    if(end < start): print "you have start and end backward!"
    if(end >= len(fastsumbuffer[xexponent][yexponent])): 
      print "you are summing up to ", end, " but your buffer has length ", len(fastsumbuffer[xexponent][yexponent])
      raise 


def slowsum(datapoints, start, end,xexponent, yexponent):
  """ this computes a range sum the old way, for unit testing """
  sum = 0.0
  for x in xrange(start,end):
    sum += (datapoints[x][0]**xexponent) * (datapoints[x][1]**yexponent)
  return sum


def bestfit(data,fastsumbuffer, start, end, degree):
  """ given a fastsumbuffer, we can find the best polynomial matching
  any area of the data in constant time."""
  if(degree >= end-start) : 
    print "*****I'M killing some degrees!!! from ",degree,"to", (end-start-1)
    degree=end-start-1
  matrix = zeros([degree+1, degree+1],'d')
  for k in xrange(degree+1) :
    for j in xrange(degree+1) :
        matrix[j][k] = fastsum(fastsumbuffer,start,end,k+j,0)# / normfactor
  vector = zeros(degree+1,'d')
  for k in xrange(degree+1):
      vector[k] = fastsum(fastsumbuffer,start,end,k,1)#/normfactor
  answer = __solve(matrix,vector)#linear_least_squares(matrix,transpose( vector))[0]
  return answer

def slowbestfit(data, start, end, degree):
  matrix = zeros([degree+1, degree+1],'d')
  for k in xrange(degree+1) :
    for j in xrange(degree+1) :
        matrix[j][k] = slowsum(data,start,end,k+j,0)# / normfactor
  vector = zeros(degree+1,'d')
  for k in xrange(degree+1):
      vector[k] = slowsum(data,start,end,k,1)#/normfactor
  answer = __solve(matrix,vector)#linear_least_squares(matrix,transpose( vector))[0]
  return answer

def slowinterpolate(data,start,end,degree,x):
  coefficients = slowbestfit(data,start,end,degree)
  y = 0.0
  for i in range(len(coefficients)):
      y += coefficients[i]* x**i
  return y

def __solve(a,v):
  return linear_least_squares(a,transpose( v))[0]
  if(len(a)==1):
    if(abs(a[0][0])<0.00001): print "WWWWWWWWWWOOOOOOOOOO flat instability"
    assert abs(a[0][0]) > 0.0
    return [ 1/a[0][0] *v[0]]
  if(len(a)==2):
    det =(a[0][0]*a[1][1]-a[0][1]*a[1][0]) 
    if(abs(det) < 1E-10): print "WWWWWWWWWWOOOOOOOOOO linear instability"
    assert abs(det) > 0.0
    return [-(a[0][1]*v[1]-a[1][1]*v[0])/ det   ,     (a[0][0]*v[1]-a[1][0]*v[0])/det]
  if(len(a)==3):
    det = ((a[0][0]*a[1][1]-a[0][1]*a[1][0])*a[2][2]+(a[0][2]*a[1][0]-a[0][0]*a[1][2])*a[2][1]+(a[0][1]*a[1][2]-a[0][2]*a[1][1])*a[2][0])
    if(abs(det) < 0.00001) : print "WWWWWWWWWWOOOOOOOOOO quadratic instability!"
    assert abs(det) > 0.0
    return [((a[0][1]*a[1][2]-a[0][2]*a[1][1])*v[2]+(a[0][2]*a[2][1]-a[0][1]*a[2][2])*v[1]+(a[1][1]*a[2][2]-a[1][2]*a[2][1])*v[0])/ det, -((a[0][0]*a[1][2]-a[0][2]*a[1][0])*v[2]+(a[0][2]*a[2][0]-a[0][0]*a[2][2])*v[1]+(a[1][0]*a[2][2]-a[1][2]*a[2][0])*v[0])/ det, ((a[0][0]*a[1][1]-a[0][1]*a[1][0])*v[2]+(a[0][1]*a[2][0]-a[0][0]*a[2][1])*v[1]+(a[1][0]*a[2][1]-a[1][1]*a[2][0])*v[0])/ det]
  #return linear_least_squares(a,transpose( v),rcond=1E-15)[0]


    

def squaredfiterror(data,fastsumbuffer,start,end,degree):
  """ Compute the sum of the square of the errors. The result seems to 
  be sometimes negative. Probably due to numerical errors. The implementation
  is probably suboptimal and slow. """
  error = 0.0
  # next we compute something \sum (a x^2 + b x + c- y) ^2
  # my algo is probably less than optimal...
  if(degree > 1) :
    print "[WARNING] PYTHON CAN'T ALWAYS COMPUTE QUADRATIC FIT ERROR RELIABLY RIGHT NOW"
  coefs = bestfit(data,fastsumbuffer,start,end,degree)
  for d in range(len(coefs)):
    for d2 in range(len(coefs)):
      error += coefs[d] * coefs[d2] * fastsum(fastsumbuffer,start,end,d+d2,0)
    error += coefs[d] * (-1.0) * fastsum(fastsumbuffer,start,end,d,1)
  for d2 in range(len(coefs)):
    error += (-1.0) * coefs[d2] * fastsum(fastsumbuffer,start,end,d2,1)
  error += (-1.0) * (-1.0) * fastsum(fastsumbuffer,start,end,0,2)
  return positive(error)
  
def segmentationsquaredfiterror(data,buffer,tags,verbose=False):
  #tags must have the format [(x0,x1,degree),(x1,x2,degree)...]
  error= 0.0
  for interval in tags:
    if(verbose):
      print "[segmentationsquaredfiterror]", interval, "error = ", squaredfiterror(data,buffer,interval[0],interval[1]+1,interval[2])
    error += squaredfiterror(data,buffer,interval[0],interval[1]+1,interval[2])
  return error

# this is for unit testing purposes 
def testfastsums(verbose = 0) :
  """ unit testing! run this periodically! """
  datapoints = []
  import random
  random.seed(0)
  for x in xrange(10):
    datapoints.append([x,random.random()])
  degree = 4 
  # compute the buffer
  fastsumbuffer = computeBuffer(datapoints,degree)
  for xexponent in xrange(degree+1):
    # we compare the long way and the fast range sum and ensures that the two
    # match closely
    assert abs(slowsum(datapoints,3,7,xexponent,0)- fastsum(fastsumbuffer,3,7,xexponent,0)) < 0.001
    assert abs(slowsum(datapoints,3,7,xexponent,1)- fastsum(fastsumbuffer,3,7,xexponent,1)) < 0.001
    assert abs(slowsum(datapoints,0,10,xexponent,0)- fastsum(fastsumbuffer,0,10,xexponent,0)) < 0.001
    assert abs(slowsum(datapoints,0,10,xexponent,1)- fastsum(fastsumbuffer,0,10,xexponent,1)) < 0.001


# this is for unit testing purposes
def testbestfit() : 
  """ unit testing"""
  datapoints = []
  # we gather data from a polynomial and try to get the polynomial back!
  # neat, no?
  for x in xrange(100):
    x = float(x)
    datapoints.append([float(x), 0.001*float(x)**2.0- float(x) + 3.0])
  #print datapoints
  coefs = bestfit(datapoints,computeBuffer(datapoints,3),10,90,2)
  # next we make sure we got the right values!
  #print coefs
  assert abs(coefs[0]-3)<0.001
  assert abs(coefs[1]+1)<0.001
  assert abs(coefs[2]-0.001)<0.0000001
  #
  #coefs = bestfit(datapoints,computeBuffer(datapoints,3),70,90,2)
  #print "mycoefs", coefs
  # next we make sure we got the right values!
  #print coefs
  #assert abs(coefs[0]-3)<0.001
  #assert abs(coefs[1]+1)<0.001
  #assert abs(coefs[2]-0.001)<0.0000001

