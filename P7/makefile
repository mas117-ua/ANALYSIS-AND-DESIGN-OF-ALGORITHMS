#MIHAI ALEXANDRU STEFANOIU 

OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_greedy

mcp: mcp_greedy.cc
	g++ ${OPTS} -o mcp_greedy mcp_greedy.cc

tar: mcp_greedy.cc makefile
	tar cvfz Y1646718R.tar.gz mcp_greedy.cc makefile

clean:
	rm -f mcp_greedy

