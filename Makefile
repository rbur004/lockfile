BIN=/usr/local/bin
LIB=/usr/local/lib

all: lockfile

lockfile: lockfile.c 
	cc -L ${LIB} -o lockfile lockfile.c -llockfile

install: all
	cp lockfile ${BIN}
