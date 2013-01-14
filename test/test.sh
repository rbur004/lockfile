#!/bin/sh
LOCKFILE=/tmp/thisScriptLock.lock
/usr/local/bin/lockfile ${LOCKFILE} $$
if [ $? != 0 ] ; then  exit 0 ; fi

#Script

/bin/rm -f ${LOCKFILE}
