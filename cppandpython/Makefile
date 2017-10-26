# This makefile assumes a POSIX compliant system (linux)
#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h
#
#

VPATH = .
MARCH = $(shell if [ `uname -m` = "x86_64" ]; then echo "athlon64"; else echo "i686"; fi)

all: multidegree

zip:
	zip -9 siamdm2007_code.zip cppmultidegree.py  README                tagstofunction.py fastpolyfit.py     segmentationplot.py   temperatureparser.py multidegree.cpp    multidegree.h      stockmarketparser.py makefile segmentandplot.py x.txt


tags: 
	ls * -1d | xargs ctags -aR

multidegree: multidegree.cpp multidegree.h
	$(CXX) -Wextra  -Wall -O2 -g2  -march=native -o multidegree multidegree.cpp
testaccuracy: testaccuracy.cpp multidegree.h
	$(CXX) -Wextra -Wall -O2 -g2  -march=native -o testaccuracy testaccuracy.cpp
multidegreedebug: multidegree.cpp 
	$(CXX) -Wextra  -g3  -Wall  -o multidegreedebug multidegree.cpp


test: multidegree
	@echo "I will segment the following data:"
	cat testdata.txt
	@echo "First, using only 3 degrees of freedom"
	./multidegree -k 3 -d 2   < testdata.txt
	@echo "Next, using 6 degrees of freedom"
	./multidegree -k 6 -d 2   < testdata2.txt
	./multidegree -k 6 -d 2 --topdownlinear  < testdata2.txt
	./multidegree -k 6 -d 2 --topdownlinear --alternate --verbose < testdata2.txt

.PHONY : clean tags test


clean :
	rm -f *.o
	rm -f multidegree
	rm -f *.a
	rm -f tags
