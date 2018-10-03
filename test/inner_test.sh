#!/bin/sh

LOCKFILE=/tmp/thisScriptLock.lock

/bin/echo "Inner Test. Lock should be taken already, so this should fail"

../lockfile ${LOCKFILE} $$
if [ $? != 0 ] ; then 
  /bin/echo "Good: Failed to get the lock for PID " $$
  exit 0
fi

/bin/echo "Bad: Should not get the lock, but have"
#Script
/bin/echo "PID of this script is " $$
/bin/echo -n "PID in Lockfile is "
/bin/cat ${LOCKFILE}

/bin/rm -f ${LOCKFILE}
/bin/echo
