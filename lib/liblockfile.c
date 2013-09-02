#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include "../include/lockfile_p.h"

#if defined(__linux__)
static int __lock(int fd, int lock_it )
{
struct flock fl;
    
  fl.l_type   = lock_it == 1 ? F_WRLCK:F_UNLCK;  /* F_RDLCK, F_WRLCK, F_UNLCK    */
  fl.l_whence = SEEK_SET; /* SEEK_SET, SEEK_CUR, SEEK_END */
  fl.l_start  = 0;        /* Offset from l_whence         */
  fl.l_len    = 0;        /* length, 0 = to EOF           */
  fl.l_pid    = getpid(); /* our PID                      */

  return fcntl(fd, F_SETLKW, &fl);  /* F_GETLK, F_SETLK, F_SETLKW */
}
static int sleep_time = 2;
#endif //__linux__

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
#if defined(__linux__)
    if((fd = open(filename, O_CREAT | O_EXCL | O_RDWR , 0x700)) ==  -1)
#else /*Assuming BSD*/
    if((fd = open(filename, O_CREAT | O_EXCL | O_RDWR | O_EXLOCK, 0x700)) ==  -1)
#endif
    {
      if(errno == EEXIST)
      {
#if defined(__linux__)
        sleep(sleep_time); //don't want grab the lock before the creating process
        //better on BSD, which has locking built into the open.
        if( (fd = open(filename, O_RDWR, 0x700)) != -1)
#else /*Assuming BSD/Mac OS X if not __linux__*/
        if( (fd = open(filename, O_RDWR |  O_EXLOCK | O_NONBLOCK, 0)) != -1)
#endif
        {
        char buff[16];  /*buffer to read the pid from the lockfile into*/
        int pid;    /*pid from the lockfile*/
        int l;      /*return code from the read call*/
        int r;      /*return result from the kill call*/
#if defined(__linux__)
          if(__lock(fd, 1) == 0)
          {
#endif
          if((l = read(fd, buff, 16)) != -1 
          && (pid = atoi(buff)) > 1 
          && (r = kill(pid, 0)) ==  -1
          && errno == ESRCH )
          {
            lseek(fd, 0, SEEK_SET);
            write(fd, pid_str, strlen(pid_str) + 1);
            write(fd, "\n", 1);
#if defined(__linux__)
            __lock(fd, 0);
#endif
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
#if defined(__linux__)
          }
          else if(errno == EAGAIN || errno == EACCES ) //failed to get the lock
          { close(fd);
            sleep(sleep_time);
            continue;
          }
          else if(debug)
          {
             printf("lock error = %s\n", strerror(errno));
          }
#endif
          close(fd);  
          return -1;
        } 
        else if(errno == ENOENT)
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
#if defined(__linux__)
      if(__lock(fd, 1) == 0)
      {
#endif
      write(fd, pid_str, strlen(pid_str) + 1);
      write(fd, "\n", 1);
#if defined(__linux__)
      __lock(fd, 0);
      fchmod(fd, 0700); //Linux oddity. O_EXCL changes the mode, and we need to fix it.
#endif
      close(fd);
      return 0;
#if defined(__linux__)
      }
        else if(debug)
        printf("Created lockfile, but failed to get lock. Error = %s\n", strerror(errno));
      sleep(sleep_time); 
#endif
    }
  }
  return -1;
}
