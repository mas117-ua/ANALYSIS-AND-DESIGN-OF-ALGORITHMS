#MIHAI ALEXANDRU STEFANOIU 

OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_bb

mcp: mcp_bb.cc
	g++ ${OPTS} -o mcp_bb mcp_bb.cc

tar: mcp_bb.cc makefile
	tar cvfz Y1646718R.tar.gz mcp_bb.cc makefile

clean:
	rm -f mcp_bb

