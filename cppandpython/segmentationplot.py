import os
from fastpolyfit import *
try:
  import Gnuplot
except ImportError:
  print "Install and download the Gnuplot.py"


g=Gnuplot.Gnuplot(debug=False)
g("set samples 1000")
g("set size 0.5")

def plotSegmentation(data,tags,filename,wait=True):
  # first figure out the max poly
  degree = max([ d for (x,y,d) in tags])
  fastsumbuffer = computeBuffer(data, degree)
  plotDataPointsWithSegmentation(data,fastsumbuffer,tags,filename,wait)

def plotDataPoints(data):
  g.plot(Gnuplot.Data(data))
  # here we could also produce a hard copy, TODO
  raw_input("press any key")
  print 

def plotDataPointsWithFunction(data, fonction):
  maxx = max(data)[0]
  minx = min(data)[0]
  g("set xrange["+str(minx)+":"+str(maxx)+"]")
  g("set grid noxtics nox2tics")
  g.plot(Gnuplot.Data(data,title="data points"),Gnuplot.Data([(x,fonction(x)) for (x,y) in data], title="function"))
  raw_input("waiting")


  
def plotDataPointsWithSegmentation(data,fastsumbuffer,tags,filename="datapointswithsegmentation.eps",wait=True):
  """ To see how to use this function, see above for plotSegmentation,
  the trick is having access to the fastsumbuffer and the tags (which
  is just a description of the segmentation)"""
  tags.sort()
  maxdegree, mindegree = 0,999
  for t in tags:
    if(t[2] > maxdegree): maxdegree = t[2]
    if(t[2] < mindegree): mindegree = t[2]
  print "min, max = ", mindegree,maxdegree
  if(maxdegree == mindegree):
    if(maxdegree == 0):
      modelname = "piecewise flat"
    elif(maxdegree == 1):
      modelname = "piecewise linear"
  else:
    if((maxdegree == 1) and (mindegree == 0)):
      modelname ="adaptive (flat,linear)"
  assert(len(tags)<60), "your segmentation is too rich for gnuplot "+str(tags)
  fncstring = ""
  for interval in tags:
    if(interval[1] < len(data)):
      fncstring += " (x <= "+str(data[interval[1]][0])+") ? "#&& ( x > "+str(interval[0])+") ? "
    else:
      fncstring += " (x <= "+str(data[-1][0])+" ) ? "
    fncstring += _tostring(data,fastsumbuffer,interval)+ " : "
    fncstring += "("
  fncstring += "0" #function is zero everywhere else
  for interval in tags : fncstring+= " ) "
  #print "here is my approx. function: ", fncstring
  #next we set the ticks so we can see where the segmentation points are!!!!
  tics = ""
  for interval in tags:
    tics += "\"\" "+str(data[interval[0]][0])+","
  #tics += "\"\" "+str(data[tags[len(tags)-1][1]][0])
  tics += "\"\" "+str(data[-1][0])
  #print "tics at"+tics
  g("set x2tics ("+tics+")")
  maxx = max(data)[0]
  minx = min(data)[0]
  g("set xrange["+str(minx)+":"+str(maxx)+"]")
  g("set x2range["+str(minx)+":"+str(maxx)+"]")
  #g("set grid noxtics x2tics")
  # ok now we plot!
  # here we could also produce a hard copy, TODO
  #g.plot(Gnuplot.Data(data,title="data points"),Gnuplot.Data(fncstring,title="piecewise polynomial approximation"))
  g("set pointsize 0.2")
  print "len(data) = ", len(data)
  if(len(data)>150):
    factor = len(data)/150
    newdata = []
    for i in range(0,len(data),factor):
      newdata.append(data[i])
    data = newdata
  g.plot(Gnuplot.Data(data,with="points"),fncstring+" ti \""+modelname+"\" lt 1 lw 2")#,title="data points"
  g.hardcopy(filename, enhanced=1, color=0)
  print os.popen("epstopdf "+filename).read()
  print "copy of plot at ",filename, "\n\n"
  if(wait): raw_input("press any key")
  return fncstring

def _tostring(data,fastsumbuffer,interval):
  """ special helper function for plotDataPointsWithSegmentation """
  coefficients = bestfit(data,fastsumbuffer,interval[0],interval[1],interval[2])
  fnc = ""
  for i in range(len(coefficients)):
    fnc += str(coefficients[i])+" * x**"+str(i)
    if(i <> len(coefficients)-1): fnc+=" + "
  return fnc

