#MIHAI ALEXANDRU STEFANOIU

OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp

mcp: mcp.cc
	g++ ${OPTS} -o mcp mcp.cc

tar: mcp.cc makefile
	tar cvfz Y1646718R.tar.gz mcp.cc makefile

clean:
	rm -f mcp

