BIN=/usr/local/bin
LIB=lib

all: lockfile
  
${LIB}/liblockfile.a:
	make -C lib

lockfile: lockfile.c ${LIB}/liblockfile.a
	cc -L ${LIB} -o lockfile lockfile.c -llockfile

clean:
	make -C lib clean
	rm -f lockfile *.o

install: all
	make -C lib install
	cp lockfile ${BIN}
