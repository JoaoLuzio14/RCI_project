# makefile
ndn: ndn.c connectivity.c auxfunctions.c ndn.h
	gcc ndn.c connectivity.c auxfunctions.c ndn.h -o ndn