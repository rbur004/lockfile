#!/bin/sh
LOCKFILE=/tmp/thisScriptLock.lock

/bin/echo "locking using file " ${LOCKFILE}
../lockfile ${LOCKFILE} $$
if [ $? != 0 ] ; then
  /bin/echo "Bad: Should have gotten the lock"
  exit 0
fi

/bin/echo "Good: Should get the lock"
#Script
/bin/echo "PID of this script is " $$
/bin/echo -n "PID in Lockfile is "
/bin/cat ${LOCKFILE}

/bin/echo "Exiting without removing the lock"
/bin/echo

