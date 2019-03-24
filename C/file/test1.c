#include<stdio.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define TMP_FILE "./test.txt"

int main()
{
 int fp;
 ssize_t len;
 char buffer[BUF_SIZE];

 fp=open(TMP_FILE,O_RDONLY);

 if (fp == -1) 
 {
  perror ("open");
  return 1;
 }

 
 printf("\n");
 while((len = read (fp, &buffer, BUF_SIZE)) > 0)
 {
  int i=0;
  for(;i<len;i++)
  {
  printf("%c",buffer[i]);
  }
 printf("\n");
 }



 close(fp);
}
