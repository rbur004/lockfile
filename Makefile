BIN=/usr/local/bin
LIB=/usr/local/lib

all: lockfile

lockfile: lockfile.c ${LIB}/liblockfile.a
	cc -L ${LIB} -o lockfile lockfile.c -llockfile

clean:
	rm -f lockfile *.o

install: all
	cp lockfile ${BIN}
