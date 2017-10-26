from fastpolyfit import *

class intervalfunction:
  def __init__(self,data,fastsumbuffer,interval):
    self.coefficients = bestfit(data,fastsumbuffer,interval[0],interval[1],interval[2])
  def f(self,x): 
    y = 0.0
    for i in range(len(self.coefficients)):
      y += self.coefficients[i]* x**i
    return y

#def tagstofunction(fastsumbuffer,tags):
#  def f(x): 
#    fncs = {}
#    for interval in tags:
#      fncs[tags[1]] = _tofunction(fastsumbuffer,interval)
#    return fncs
#  return f
class tagstofunction:
  def __init__(self,data,fastsumbuffer,tags):
    self.fncs = []
    #print tags
    for interval in tags:
      self.fncs.append((data[interval[1]][0] ,intervalfunction(data,fastsumbuffer,interval)))
    #for interval in tags:
    #  print self.f(data[interval[0]][1],verbose=True)
  def f(self,x,verbose=False):
    if(verbose): print "x = ", x
    for (interval,fnc) in self.fncs:
      if(verbose) : print " x< ",interval," ?"
      if x <= interval:
        if(verbose): print "yes!"
        return fnc.f(x)
    print "not good",[x, interval,self.fncs.keys()]
    return  fnc.f(x)


