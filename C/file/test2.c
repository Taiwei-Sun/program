#include<stdio.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define TMP_FILE "./test.txt"

int main()
{
 int fp;
 ssize_t len=3;
 //char *ch="abc";
 char ch[]="123";

 fp=open(TMP_FILE,O_RDWR);

 if (fp == -1) 
 {
  perror ("open");
  return 1;
 }

 write(fp, &ch, (ssize_t)len);//char ch[]="123"; can use ch and &ch,  char *ch="abc"; only use ch



 close(fp);
}
