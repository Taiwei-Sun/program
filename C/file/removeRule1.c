#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define TMP_FILE "./test.txt"

int main()
{
 int fp;
 ssize_t len=1;
 char ch='\0';
 off_t n;

 fp=open(TMP_FILE,O_RDWR);

 if (fp == -1) 
 {
  perror ("open");
  return 1;
 }

 n=lseek(fp,0,SEEK_END);
 //printf("n=%d",n);

 //write(fp, &ch, (ssize_t)len);

 ftruncate(fp,n-5);

 close(fp);
}
