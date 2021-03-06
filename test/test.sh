#!/bin/sh
LOCKFILE=/tmp/thisScriptLock.lock

./lock_and_leave.sh

/bin/echo "locking using file " ${LOCKFILE}
/bin/echo "Lock file should exist, and we will steal it, as previous process ended"
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

/bin/echo
./inner_test.sh

/bin/rm -f ${LOCKFILE}
