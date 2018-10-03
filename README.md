# lockfile

* Source https: https://github.com/rbur004/lockfile

## DESCRIPTION:

Lockfile creates a named lockfile and stores the PID of the process in it, so we can ensure the process holding the lock is still running.

## FEATURES/PROBLEMS:


## SYNOPSIS:
 
lockfile [-d] <filename> <pid>
-d debug mode. 
filename is the lockfiles path and name
pid is the process id to store in the file
    The pid is tested with 
		kill 0 pid
	to see if the process holding the lock is still running.
	
Returns 0 if we got the lock
        -1 if there is an error

We try 10 times to get the lock, then give up
If the Process holding the lock is not running, we steal the lock.

## REQUIREMENTS:

	Unix/Linux/Mac OS X
	
## INSTALL:

* make
* make install
  Installs into /usr/local/bin
  
## TEST:

  Simple check in test/. 
* cd to test and run ./test.sh
* This will create a lockfile with ./lock_and_leave.sh
* Then ./test.sh will steal the lock, as ./lock_and_leave.sh will no longer be running
* Then it runs ./inner_test.sh, which will fail to get the lock, as ./test.sh has the lock.

## LICENSE:

(The MIT License)

Copyright (c) 2013

1. You may make and give away verbatim copies of the source form of the
   software without restriction, provided that you duplicate all of the
   original copyright notices and associated disclaimers.

2. You may modify your copy of the software in any way, provided that
   you do at least ONE of the following:
    *  place your modifications in the Public Domain or otherwise make them Freely Available, such as by posting said modifications to Usenet or an equivalent medium, or by allowing the author to include your modifications in the software.
    *  use the modified software only within your corporation or organization.
    *  rename any non-standard executables so the names do not conflict with standard executables, which must also be provided.
    *  make other distribution arrangements with the author.

3. You may distribute the software in object code or executable form, provided that you do at least ONE of the following:
    * distribute the executables and library files of the software,
  together with instructions (in the manual page or equivalent)
  on where to get the original distribution.
    * accompany the distribution with the machine-readable source of
  the software.
    * give non-standard executables non-standard names, with
        instructions on where to get the original software distribution.
    * make other distribution arrangements with the author.

4. You may modify and include the part of the software into any other
   software (possibly commercial).  But some files or libraries used by
   code in this distribution  may not written by the author, so that they 
   are not under these terms.

5. The scripts and library files supplied as input to or produced as 
   output from the software do not automatically fall under the
   copyright of the software, but belong to whomever generated them, 
   and may be sold commercially, and may be aggregated with this
   software.

6. THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
   IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
   PURPOSE.
