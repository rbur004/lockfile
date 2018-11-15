#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h> //needed for atoi on Mac OS X
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h> //Linux compile needed this for fchmod
#include "../include/lockfile_p.h"

#if defined(__linux__)

static int __lock(int fd, int lock_it) {
  struct flock fl;

  fl.l_type = lock_it == 1 ? F_WRLCK : F_UNLCK; /* F_RDLCK, F_WRLCK, F_UNLCK   */
  fl.l_whence = SEEK_SET;                      /* SEEK_SET, SEEK_CUR, SEEK_END */
  fl.l_start = 0;                              /* Offset from l_whence         */
  fl.l_len = 0;                               /* length, 0 = to EOF            */
  fl.l_pid = getpid();                        /* our PID                       */

  return fcntl(fd, F_SETLKW, & fl);          /* F_GETLK, F_SETLK, F_SETLKW */
}
static int sleep_time = 2;

#endif //__linux__

/*Attempts to create a lockfile and store the pid_str in it.
  Returns 0 if it succeeds.
         -1 if it fails
*/
#if defined(__linux__)

//No lock on open under linux, so we open then lock.
int lockfile_p(char *filename, char *pid_str, int debug) {
  int fd; /*File descriptor of lockfile*/
  int i; /*Loop index*/
  
  for (i = 0; i < 10; i++) {
    //open with no access to other callers (by creating a file with mode 0), hence mimic BSD O_EXLOCK.
    if ((fd = open(filename, O_CREAT | O_EXCL | O_RDWR, 0)) == -1) { 
      if (errno == EEXIST) { //File already existed
        if ((fd = open(filename, O_RDWR, 0)) != -1) { //Create failed, so open the file that is already there.
          char buff[16]; /*buffer to read the pid from the lockfile into*/
          int pid; /*pid from the lockfile*/
          int l; /*return code from the read call*/
          int r; /*return result from the kill call*/
                    
          if (__lock(fd, 1) == 0) {
            if ((l = read(fd, buff, 16)) != -1 && (pid = atoi(buff)) > 1 && (r = kill(pid, 0)) == -1 && errno == ESRCH) {
              //Lock file existed, but the process holding it is no longer running. So we steal the lock.
              lseek(fd, 0, SEEK_SET);
              write(fd, pid_str, strlen(pid_str) + 1);
              write(fd, "\n", 1);
              __lock(fd, 0);
              close(fd);
              return 0; //We have the lock.
            } else if (debug == 1) {
              if (l == -1) 
                printf("read error = %s\n", strerror(errno));
              if (r == -1) 
                printf("kill error = %s\n", strerror(errno));
              else 
                printf("kill: Process %d is still running\n", pid);
            }
          } else if (errno == EAGAIN || errno == EACCES) {//failed to get the lock as it was still being created or was locked
            close(fd);
            sleep(sleep_time);
            continue; //Try again.
          } else if (debug)
            printf("lock error = %s\n", strerror(errno));
          close(fd);
          return -1;
        } else if (errno == ENOENT) 
          continue; //Try again, as the lock file has gone.
        else if (debug) 
          printf("2nd open error = %s\n", strerror(errno));
      } else if (debug == 1) 
        printf("Open error = %s\n", strerror(errno));
      return -1; //We didn't get the lock.
    } else { //We created the lock file, so now we write the PID into it. 
      //no need to lock it, as the permission were set to ---------, so everyone else is going to get an EACCES error.
      write(fd, pid_str, strlen(pid_str) + 1);
      write(fd, "\n", 1);
      fchmod(fd, 0600); //Linux oddity. No O_EXLOCK on open, so we changed the permission from unreadable on create to rw------- here.
      close(fd);
      return 0;  //We have the lock.
    }
  }
  return -1; //We didn't get the lock.
}

#else /* Assuming BSD / Mac OS X, where we can lock on open, so code is much simpler than the linux case */

int lockfile_p(char *filename, char *pid_str, int debug) {
  int fd; /*File descriptor of lockfile*/
  int i; /*Loop index*/
  
  for (i = 0; i < 10; i++) {
    if ((fd = open(filename, O_CREAT | O_EXCL | O_RDWR | O_EXLOCK, 0700)) == -1) //locked on open.
    { //open failed. 
      if (errno == EEXIST) //File already existed, so the lock is taken.
      {
        if ((fd = open(filename, O_RDWR | O_EXLOCK | O_NONBLOCK, 0)) != -1) {//open, with a read lock, failing immediately if we can't get it
          char buff[16]; /*buffer to read the pid from the lockfile into*/
          int pid; /*pid from the lockfile*/
          int l; /*return code from the read call*/
          int r; /*return result from the kill call*/
          
          if ((l = read(fd, buff, 16)) != -1 && (pid = atoi(buff)) > 1 && (r = kill(pid, 0)) == -1 && errno == ESRCH) {
            //Read the PID, and found the process is not running, so we are going to steal the lock.
            lseek(fd, 0, SEEK_SET);
            write(fd, pid_str, strlen(pid_str) + 1);
            write(fd, "\n", 1);
            close(fd);
            return 0; //We have the lock.
          } else if (debug == 1) {
            if (l == -1) 
              printf("read error = %s\n", strerror(errno));
            if (r == -1) 
              printf("kill error = %s\n", strerror(errno));
            else 
              printf("kill: Process %d is still running\n", pid);
          }
          close(fd);
          return -1; //We didn't get the lock.
        } else if (errno == ENOENT) //Lock file gone, so we will try again.
          continue;
        else if (debug) 
          printf("2nd open error = %s\n", strerror(errno));
      } else if (debug == 1) 
        printf("Open error = %s\n", strerror(errno));
      return -1; //We didn't get the lock.
    } else { //We just created the lockfile.
      write(fd, pid_str, strlen(pid_str) + 1);
      write(fd, "\n", 1);
      close(fd);
      return 0; //We have the lock.
    }
  }
  return -1; //We failed to get the lock.
}

#endif /* End of BSD Section */
