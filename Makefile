BINDEST=${RPM_BUILD_ROOT}/usr/local/bin
LIBSRC=lib

all: lockfile
  
${LIBSRC}/liblockfile.a:
	make -C lib

lockfile: lockfile.c ${LIBSRC}/liblockfile.a
	cc -L ${LIBSRC} -o lockfile lockfile.c -llockfile

clean:
	make -C lib clean
	rm -f lockfile *.o

install: all
	make -C lib install
	make -C include install
	cp lockfile ${BINDEST}
