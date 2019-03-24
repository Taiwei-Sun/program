#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024
#define TMP_FILE "./test.txt"
#define RULE "sshd: 127.0.0.1"

int main()
{
 int fp;
 char sshRule[]=RULE;
 ssize_t len=strlen(sshRule);
 off_t end_point;

 fp=open(TMP_FILE,O_RDWR|O_APPEND);

 if (fp == -1) 
 {
  perror ("open");
  return 1;
 }

 end_point=lseek(fp,0,SEEK_END);
 //printf("len=%d",len);

 write(fp, sshRule, (ssize_t)len);

 //do something

 ftruncate(fp,end_point);

 close(fp);
}
