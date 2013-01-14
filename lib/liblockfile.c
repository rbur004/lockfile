#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "../include/lockfile_p.h"

/*Attempts to create a lockfile and store the pid_str in it.
  Returns 0 if it succeeds.
         -1 if it fails
*/
int lockfile_p(char *filename, char *pid_str, int debug)
{
int fd; /*File descriptor of lockfile*/
int i;  /*Loop index*/
	for(i = 0; i < 10; i++)
	{
		if((fd = open(filename, O_CREAT | O_EXCL | O_RDWR | O_EXLOCK, 0x700)) ==  -1)
		{
			if(errno == EEXIST)
			{
				if( (fd = open(filename, O_RDWR |  O_EXLOCK | O_NONBLOCK, 0)) != -1)
				{
				char buff[16];	/*buffer to read the pid from the lockfile into*/
				int pid; 		/*pid from the lockfile*/
				int l;			/*return code from the read call*/
				int r; 			/*return result from the kill call*/
					if((l = read(fd, buff, 16)) != -1 
					&& (pid = atoi(buff)) > 1 
					&& (r = kill(pid, 0)) ==  -1
					&& errno == ESRCH )
					{
						lseek(fd, 0, SEEK_SET);
						write(fd, pid_str, strlen(pid_str) + 1);
						write(fd, "\n", 1);
						close(fd);
						return 0;
					}	
					else if(debug == 1)
					{
						if(l == -1)
							printf("read error = %s\n", strerror(errno));
						if(r == -1)
							printf("kill error = %s\n", strerror(errno));
						else
							 printf("kill: Process %d is still running\n", pid);
					}
				}	
				else if(fd == ENOENT)
					continue;
				else if(debug)
					printf("2nd open error = %s\n", strerror(errno));
			}
			else if(debug == 1)
				printf("Open error = %s\n", strerror(errno));
			return -1;
		}
		else
		{
			write(fd, pid_str, strlen(pid_str) + 1);
			write(fd, "\n", 1);
			close(fd);
			return 0;
		}
	}
}
