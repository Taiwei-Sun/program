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
 ssize_t len=5;
 char ch[]="\nabc\n";
 

 fp=open(TMP_FILE,O_RDWR|O_APPEND);

 if (fp == -1) 
 {
  perror ("open");
  return 1;
 }


 write(fp, ch, (ssize_t)len);


 close(fp);
}
