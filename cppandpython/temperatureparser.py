import re
import time


def readCDIAC(filename):
  """ as the name suggests
  http://cdiac.esd.ornl.gov/ftp/trends/temp/lugina/0-60S.dat
  """
  regex = r"([0-9]+)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)\s+(-?[0-9]*\.[0-9]*)"
  # date format seems to be month/day/year
  data = []
  for i in range(12): 
          data.append([])
  in_file =open(filename,'r')
  for line in in_file:
    #print line
    matchedcontent = re.match(regex,line)
  #  #print matchedcontent
    if(matchedcontent<> None):
  #    #print matchedcontent.group(1)
      year = int(matchedcontent.group(1))
      #print date
      for i in range(12):
              data[i].append((year,float(matchedcontent.group(2+i))))
  #print "read ", len(data), " times 12 data points!"
  in_file.close()
  assert len(data)>0
  data.sort()
  #data.reverse()
  return data


if __name__ == "__main__":
  print readCDIAC("../data/90N-60S.dat")

