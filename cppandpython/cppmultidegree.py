import random
import re
import os
import segmentationplot
import stockmarketparser
import temperatureparser
import time
import fastpolyfit
from tagstofunction import tagstofunction
from math import sqrt
import sys
#import errorplot 


def cppprocess(data,command,debug=False):
  #print data
  cmd = os.path.abspath("multidegree")+" "+command
  print "running: ",cmd, " on ", len(data), " data points"
  if(debug):
    debugname = "debug"+str(int(time.time()))+".txt"
    print "dumping data points to file "+debugname
    debugf = open(debugname,"w")
    for point in data:
      debugf.write(str(point[0])+","+str(point[1])+"\n")
    debugf.flush()
    debugf.close()
  outstream,instream = os.popen2(cmd)
  if outstream:
                  for point in data:
                     outstream.write(str(point[0])+","+str(point[1])+"\n")
                  outstream.flush()
                  outstream.close()
  answer =  instream.readlines()
  tags = []
  for line in answer:
    #print line
    if(line.startswith("#")): 
      errors = re.findall("# error (.*)", line)
      if(len(errors) > 0): 
        error = float(errors[0])
        print "error = ", error, float(error)/len(data)
      else:
        print line
      continue
    #print line
    map(int,line.split(" "))
    newdata = map(int,line.split(" "))
    tags.append((newdata[0],newdata[1],newdata[2]))
  return error, tags

def leaveoneout(data,command, samples = 100):
  sum = 0.0
  skip = max(1,len(data)/samples)
  counter = 0
  print "leavoneout with ",len(data), " data points"
  for i in range(1,len(data)-2,skip):
  #for i in [1]:
    minusonedata = data[:i]+data[i+1:]
    #print "i = ", i, "sending ",len(minusonedata)," data points to cpp"
    #buf = fastpolyfit.computeBuffer(minusonedata,2)
    error, tags = cppprocess(minusonedata,command)
    counter += 1
    #segmentationplot.plotSegmentation(data,tags,"garbage.eps",wait=True)
    #print tags
    #print i, data[i]
    #fnc = tagstofunction(minusonedata,buf,tags)
    #print fnc.f(data[i][0])
    rightinter = None
    #print "len(minusonedata)=",len(minusonedata)
    #if(data[i][0] <= minusonedata[tags[0][1]][0]):
    #  rightinter = tags[0]
    #else:
    rightinter = None
    for inter in tags[:-1]:
      #print data[i][0]
      #assert(inter[0] < inter[1])
      #print "begin=",inter[0],minusonedata[inter[0]][0]
      #print data[i][0]
      #print "end=",inter[1],minusonedata[inter[1]][0]
      if((data[i][0] >= minusonedata[inter[0]][0]) and (data[i][0] <= minusonedata[inter[1]][0])): 
        rightinter = inter 
        #print "ok with ", inter
      #else: 
      #  #print (data[i][0] >= minusonedata[inter[0]][0])
      #  #print (data[i][0] <= minusonedata[inter[1]][0])
      #  #print data[i][0]
      #  #print minusonedata[inter[1]-1][0]
      #  #print minusonedata[inter[0]][0] 
      #  #print "not ok with ",inter 
    if(rightinter == None):
      rightinter = tags[-1]
      #print "i",i
      #print data[i][0]
      #print rightinter
      #print "first interval = ", minusonedata[tags[0][0]][0], minusonedata[tags[0][1]][0]
      #print "second interval = ", minusonedata[tags[1][0]][0], minusonedata[tags[1][1]][0]
      assert (data[i][0] >= minusonedata[rightinter[0]][0]) 
    #print "rightinter", rightinter
    #print tags[-1]
    #print tags[-2]
    #inter = tags[-2]
    #print  minusonedata[inter[1]-1][0]
    #print minusonedata[inter[0]][0]
    #print (data[i][0] >= minusonedata[inter[0]][0])
    #print (data[i][0] <= minusonedata[inter[1]][0])
    #print data[i][0]
    loneoutfiterror =  (data[i][1] - fastpolyfit.slowinterpolate(minusonedata,start=rightinter[0],end=rightinter[1],degree=rightinter[2],x=data[i][0])) **2
    print "leave one out error is ", loneoutfiterror 
    sum += loneoutfiterror   #print len(data)
  #print skip
  print sum 
  return sqrt(sum / counter)

def cpp(data,command,waitforme=False):
  before = time.time()
  error, tags = cppprocess(data,command)
  after = time.time()
  print tags
  filename = "temp_"+re.sub(" ", "_",command)+"_.eps"
  #  print "data = ",data
  print "========================"
  #try :
  segmentationplot.plotSegmentation(data,tags,filename,wait=waitforme)
  #except IndexError:
  #  print "can't generate plot, index error"
  print after-before
  return error,after-before,tags


def ecgparser(filename):    
    f = open(filename)
    data = []
    for line in f:
      dataentry = map(int,re.findall("[0-9]+",line))
      if(len(dataentry) >1):
        data.append((dataentry[0],dataentry[1]))
    f.close()
    return data
    
  

def normalizex(data):
  for i in range(len(data)):
    data[i]=(i,data[i][1])
    
if __name__ == "__main__":
  #<<<<<<< cppmultidegree.py
  #data0 = ecgparser("/home/lemire/CVS/ylb/samples/ECG/100.txt")[:1000]
  #data1 = stockmarketparser.readyahoofinance("../data/GOOG.csv")
  #data2 = stockmarketparser.readyahoofinance("../data/SUNW.csv")
  #data3 = stockmarketparser.readyahoofinance("../data/MSFT.csv")
  #data4 = stockmarketparser.readyahoofinance("../data/DOWJONES.csv")
  #data5 = stockmarketparser.readyahoofinance("../data/NASDAQ.csv")
  #data6 = stockmarketparser.readyahoofinance("../data/AAPL.csv")
  #data7 = stockmarketparser.readyahoofinance("../data/IBM.csv")
  #data8 = stockmarketparser.readyahoofinance("../data/DELL.csv")
  #data9 = stockmarketparser.readyahoofinance("../data/HP.csv")
  #=======
  stock=False
  try:
          sys.argv.index("--stock")
          stock = True
  except: pass
  stockplus=False
  try:
          sys.argv.index("--stockplus")
          stockplus = True
  except: pass
  randomwalk=False
  try:
          sys.argv.index("--randomwalk")
          numberofwalks = int(sys.argv[sys.argv.index("--randomwalk")+1])
          randomwalk = True
  except: pass
  randomflag=False
  try:
          sys.argv.index("--random")
          numberofrandom = int(sys.argv[sys.argv.index("--random")+1])
          randomflag = True
  except: pass
  ecg = False
  oneecg = False
  temp = False
  opti = False
  onlycore = False
  timings = False
  microsoft = False
  budget = 30
  if(ecg):
            budget = 20
  if(temp):
            budget = 10
  try: 
          sys.argv.index("--microsoft")
          microsoft = True
  except: pass
  try: 
          sys.argv.index("--timings")
          timings = True
  except: pass
  try: 
          sys.argv.index("--onlycore")
          onlycore = True
  except: pass
  try: 
          sys.argv.index("--temp")
          temp = True
  except: pass
  try: 
          sys.argv.index("--ecg")
          ecg = True
  except: pass
  try: 
          sys.argv.index("--oneecg")
          oneecg = True
  except: pass
  wait=False
  leaveone=False
  try:
          sys.argv.index("--leaveone")
          leaveone = True
  except: pass
  try: 
          sys.argv.index("--wait")
          wait = True
  except: pass
  try: 
          sys.argv.index("--optimal")
          opti = True
  except: pass
  limit = 999999999999
  try :
          i = sys.argv.index("--limit")
          limit = int(sys.argv[i+1])
  except: pass
  try :
          i = sys.argv.index("--budget")
          budget = int(sys.argv[i+1])
  except: pass
  numberofsources = 999999999999
  try :
          i = sys.argv.index("--numberofsources")
          numberofsources = int(sys.argv[i+1])
  except: pass
  downsampling = 1
  try :
          i = sys.argv.index("--downsampling")
          downsampling = int(sys.argv[i+1])
  except: pass
  dtsrc= []
  if(oneecg): 
          #for i in range(5):
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/100.txt")[290:200+290][:limit])
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/101.txt")[0:200+0][:limit])
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/102.txt")[300:200+300][:limit])
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/103.txt")[200:200+200][:limit])
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/104.txt")[200:200+200][:limit])
          dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/105.txt")[100:200+100][:limit])
  if(ecg): 
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/100.txt")[i*600+250:(i+1)*600+250][:limit])
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/101.txt")[i*600+250:(i+1)*600+250][:limit])
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/102.txt")[i*600+250:(i+1)*600+250][:limit])
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/103.txt")[i*600+250:(i+1)*600+250][:limit])
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/104.txt")[i*600+250:(i+1)*600+250][:limit])
          for i in range(5):
                  dtsrc.append(ecgparser("/home/lemire/CVS/ylb/samples/ECG/105.txt")[i*600+250:(i+1)*600+250][:limit])
  if(microsoft):
    dtsrc.append(stockmarketparser.readyahoofinance("../data/MSFT.csv")[:limit])
  if(stock):
    dtsrc.append(stockmarketparser.readyahoofinance("../data/GOOG.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/SUNW.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/MSFT.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/DOWJONES.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/NASDAQ.csv")[:limit])
  if(stockplus):
    dtsrc.append(stockmarketparser.readyahoofinance("../data/AAPL.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/ADOBE.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/ATITECH.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/AUTODESK.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/CONEXANT.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/HYPERION.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/LOGITECH.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/NVIDIA.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/PALM.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/REDHAT.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/RSA.csv")[:limit])
    dtsrc.append(stockmarketparser.readyahoofinance("../data/SANDISK.csv")[:limit])
  if(randomwalk):
    for j in xrange(numberofwalks):
      dat = []
      dat.append((0,0.0))
      for i in xrange(1,200):
        v=dat[-1][1]+random.normalvariate(0,1)
        dat.append((i,v))
      dtsrc.append(dat[:limit])
  if(randomflag):
    for j in xrange(numberofrandom):
      dat = []
      for i in xrange(0,200):
        v=random.normalvariate(0,1)
        dat.append((i,v))
      dtsrc.append(dat[:limit])
  if(temp): 
    for n in ["../data/0-60S.dat","../data/0-90N.dat", "../data/90N-60S.dat"]:
            data = temperatureparser.readCDIAC(n)
            for i in range(3):
                    dtsrc.append(data[i][:limit])
  if(downsampling>1):
    def downsample(dat,factor):
      answer = []
      for i in range(0,len(dat),factor):
        answer.append(dat[i])
      return answer
    for j in range(len(dtsrc)):
      dtsrc[j] = downsample(dtsrc[j],downsampling)
      print "new length = ", len(dtsrc[j])
  dtsrc = dtsrc[:numberofsources]  
  all =  []
  l1outall = []
  log = open("cppmultidegree_"+str(int(time.time()))+".log","w")
  log.write(str(sys.argv)+"\n")
  if(timings):
    source = dtsrc[0]
    all = []
    for i in range(50,len(source)):
      data = source[:i]
      log.write("i = "+str(i)+" \n")
      error = [i]
      error.append(cpp(data,'-k '+str(budget)+' -d 2 --heuristic1',wait)[:2])
      if( not onlycore): 
            error.append(cpp(data,'-k '+str(budget)+' -d 2 --heuristic2',wait)[:2])
      error.append(cpp(data,'-k '+str(budget)+' -d 2',wait)[:2])
      if (not onlycore):
            error.append(cpp(data,'-k '+str(budget)+' -d 2 --topdown ',wait)[:2])
      error.append(cpp(data,'-k '+str(budget)+' --topdownlinear',wait)[:2])
      if (not onlycore): 
            error.append(cpp(data,'-k '+str(budget)+' --topdownflat ',wait)[:2])
      all.append(error)
      log.write("--error,time--- \n")
      log.write(str(error)+"\n")
      log.flush()
    for line in all:
      print line
  else: 
   for data in dtsrc:
    error =[]
    l1out = []
    log.write("===data source===\n");
    log.write("Budget: "+str(budget)+"\n")
    normalizex(data)
    if(leaveone):
      l1out.append( leaveoneout(data, '-k '+str(budget)+' -d 2 --heuristic1') )
      #if( not onlycore): 
      #  l1out.append( leaveoneout(data, '-k '+str(budget)+' -d 2 --heuristic2') )
      #if( not onlycore): 
      #  l1out.append( leaveoneout(data, '-k '+str(budget)+' -d 2 --topdown') )
      l1out.append( leaveoneout(data, '-k '+str(budget)+' -d 2 --topdownlinear') )
      if( not onlycore): 
        l1out.append( leaveoneout(data, '-k '+str(budget)+' -d 1 --topdownflat') )
      log.write("--leave one out--- \n")
      log.write(str(l1out)+"\n")
    l1outall.append(l1out)
    error.append(cpp(data,'-k '+str(budget)+' -d 2 --heuristic1',wait)[:2])
    #if( not onlycore): 
    #        error.append(cpp(data,'-k '+str(budget)+' -d 2 --heuristic2',wait)[:2])
    if(opti): 
      error.append(cpp(data,'-k '+str(budget)+' -dmin 1 -d 2',wait)[:2])
      error.append(cpp(data,'-k '+str(budget)+' -dmin 0 -d 2',wait)[:2])
      error.append(cpp(data,'-k '+str(budget)+' -dmin 0 -d 1',wait)[:2])
    #if (not onlycore):
    #        error.append(cpp(data,'-k '+str(budget)+' -d 2 --topdown ',wait)[:2])
    error.append(cpp(data,'-k '+str(budget)+' --topdownlinear',wait)[:2])
    #if (not onlycore): 
    error.append(cpp(data,'-k '+str(budget)+' --topdownflat ',wait)[:2])
    all.append(error)
    log.write("--error,time--- \n")
    log.write(str(error)+"\n")
    log.flush()
   for line in all:
    print line
   if(leaveone):
    print "leave one out errors "
    for line in  l1outall:
      print line
  log.close()
  def fixrange(array):
          m = max(array)[0]
          return [(e/m,t) for e,t in array]
  #errorplot.ploterrortimes([fixrange(case) for case in all])
  
  

