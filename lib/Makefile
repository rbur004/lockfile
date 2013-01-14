LIBDEST=/usr/local/lib

all: liblockfile.a

lockfile.o: liblockfile.c
	cc -c liblockfile.c

liblockfile.a: liblockfile.o
	ar -r liblockfile.a liblockfile.o
	ranlib liblockfile.a

${LIBDEST}/liblockfile.a: liblockfile.a

install: all
	cp liblockfile.a ${LIBDEST}

clean:
	rm -f liblockfile.a liblockfile.o
