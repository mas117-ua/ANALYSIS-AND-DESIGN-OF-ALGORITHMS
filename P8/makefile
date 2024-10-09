#MIHAI ALEXANDRU STEFANOIU 
OPTS = -Wall -Wextra -O3 -std=c++0x

all: mcp_bt

mcp: mcp_bt.cc
	g++ ${OPTS} -o mcp_bt mcp_bt.cc

tar: mcp_bt.cc makefile
	tar cvfz Y1646718R.tar.gz mcp_bt.cc makefile

clean:
	rm -f mcp_bt

