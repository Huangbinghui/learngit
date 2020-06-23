#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
 
int main()
{
  unsigned char buffer[100];
  int i;
  int fd;
  char key=0;
  fd = open("/dev/simple", O_RDWR);
  if (fd == -1)
    {
      perror("open device button errr!");
      return 0;
    }
  read(fd,buffer,4);
  for (i=0;i <4;i++)
  printf("%d\n",buffer[i]);
  pause();
  close(fd);
  return 0;
}