import cppmultidegree
import sys
d=[]
print sys.argv
if(len(sys.argv)<3):
  print "please specify some data file name and some budget k"
  print " example: python segmentandplot.py myfile 2"
  sys.exit(-1)
f=open(sys.argv[1])
for line in f:
  if(line.startswith("#")): continue
  sp=line.strip().split(" ")
  print line, sp
  d.append((float(sp[0]),float(sp[-1])))
cppmultidegree.cpp(d,'-k '+str(int(sys.argv[2]))+' -d 2 ',True)