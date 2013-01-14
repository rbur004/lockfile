#include <stdio.h>
#include <string.h>
#include "include/lockfile_p.h"

/*
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

*/

int main(int argc, char **argv, char **arge)
{
int debug = 0;

	if(argc == 4 && strcmp(argv[1], "-d") == 0)
	{
		debug = 1;
		printf("Debug On\n");
		argv++;
	}
	else if(argc != 3)
	{	printf("Usage: lockfile [-d] <filename> <pid>\n");
		return -1;
	}

	return lockfile_p(argv[1], argv[2], debug);
	
}
		
