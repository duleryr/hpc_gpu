PROG = main
CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -fopenmp

SRCDIR=./src
TESTDIR=./Unit_Tests
CSOURCE=$(SRCDIR)/Floyd_Warshall.cpp
CSMAIN=$(SRCDIR)/main.cpp
CSTEST=$(TESTDIR)/bloc_layout.cpp

$(PROG): 
	$(CXX) $(CSOURCE) $(CXXFLAGS) -O3 $(CSMAIN) -o $@ 
	
sinopti:
	$(CXX) $(CSOURCE) $(CXXFLAGS) $(CSMAIN) -o $@ 

debug:
	$(CXX) $(CSOURCE) $(CXXFLAGS) -g -pg $(CSMAIN) -o $@ 
	
test:
	$(CXX) $(CSTEST) $(CXXFLAGS) -o $@ 

doc:
	pdflatex -output-directory=Documentation/ Documentation/Presentacion_proyecto.tex

all: $(PROG) debug test doc sinopti

clean :
	rm -f ./main
	rm -f ./debug
	rm -f ./test
	rm -f ./sinopti
	rm -f Documentation/Presentacion_proyecto.aux
	rm -f Documentation/Presentacion_proyecto.log
	rm -f Documentation/Presentacion_proyecto.dvi
	rm -f Documentation/Presentacion_proyecto.pdf
	rm -f Output_Files/basic*.txt
	rm -f Output_Files/simple*.txt
	rm -f Output_Files/medium*.txt
	rm -f Output_Files/complex*.txt

.PHONY: all debug test clean sinopti
