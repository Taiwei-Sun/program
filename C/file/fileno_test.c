#include<stdio.h>

int main()
{
FILE *fp1,*fp2;
int fd1,fd2;
fp1=fopen("/etc/passwd","r");
fd1=fileno(fp1);

fp2=fopen("/etc/crontab","r");
fd2=fileno(fp2);


printf("fd1=%d\nfd2=%d\n",fd1,fd2);
fclose(fp1);
fclose(fp2);
}
