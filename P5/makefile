#MIHAI ALEXANDRU STEFANOIU 
OPTS = -Wall -Wextra -O3 -std=c++0x

all: pow2 pow2.Time graphs

graphs: pow2.Time pow2.gpi
	gnuplot pow2.gpi
pow2: pow2.cc
	g++ ${OPTS} -o pow2 pow2.cc	
	
pow2.Time: pow2
	./pow2 > pow2.Time
	
tar: pow2.cc pow2.gpi makefile
	tar cvfz Y1646718R.tar.gz pow2.cc pow2.gpi makefile
	
clean: 
	rm -f pow2 pow2.Time pow2*.png
