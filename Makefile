# makefile
ndn: ndn.c auxfunctions.c ndn.h
	gcc ndn.c auxfunctions.c ndn.h -o ndn