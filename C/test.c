#include <stdio.h>
#include <stdlib.h>

/*
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#ifndef STDERR_FILENO
# define STDERR_FILENO   2
#endif
*/

#define TCP_FLAGS_SYNACK_V 0x12
#define TCP_FLAGS_ACK_V 0x10
#define TCP_FLAGS_PSHACK_V 0x18


int main(int ac, char **av)
{

printf("hello world!!\n");
printf("TCP_FLAGS_SYNACK_V|TCP_FLAGS_ACK_V=%x\n",TCP_FLAGS_SYNACK_V|TCP_FLAGS_ACK_V);
printf("TCP_FLAGS_SYNACK_V&TCP_FLAGS_ACK_V=%x\n",TCP_FLAGS_SYNACK_V&TCP_FLAGS_ACK_V);
printf("TCP_FLAGS_SYNACK_V=%x TCP_FLAGS_ACK_V=%x\n",TCP_FLAGS_SYNACK_V,TCP_FLAGS_ACK_V);


/*
//test 20180515
if(ac!=2){printf("$command string\n");return 1;}

printf("you type\"%s\"\n",av[1]);
printf("transform:");
int i=0;
while(av[1][i]!='\0')
{
 if(av[1][i]<'C'){printf("must type >\'C\'\n");return 1;}
 printf("%d,",av[1][i]-'C'+1);
 i++;
}

printf("\n");
*/


/*
//test
 char s[] = "Linux Programmer!\n";
 int nullfd, dupfd;

 if ((nullfd = dupfd = open("/dev/stdout", O_RDWR)) == -1) {
  fprintf(stderr, "Couldn't open /dev/null: %s\n",
  strerror(errno));
  exit(1);
 }

 printf("dupfd=%d\n",dupfd);
 printf("F_GETFL=%d\n",F_GETFL);
 printf("EBADF=%d\n",EBADF);
 printf("errno=%d\n",errno);

 while (++dupfd <= STDERR_FILENO) {
 // Only populate closed fds. 
  if (fcntl(dupfd, F_GETFL) == -1 && errno == EBADF) {
   if (dup2(nullfd, dupfd) == -1) {
    fprintf(stderr, "dup2: %s\n", strerror(errno));
    exit(1);
   }
  }
 }


 write(nullfd, s, sizeof(s));

 if (nullfd > STDERR_FILENO)
 close(nullfd);


 printf("STDERR_FILENO=%d\n",STDERR_FILENO);
 printf("nullfd=%d\n",nullfd);
*/

/*
//test20180406
int i;
for(i=0;i<0;i++)
 printf("A\n");
printf("B\n");
*/

/*
int i=1,j=0,i1,i2,j1,j2;
//bool k=true,k1,k2;

printf("int i=%d,j=%d\n",i,j);
//printf("bool k=%d\n",k);

i1=!i;j1=!j;
printf("after !: \n");

printf("int i=%d,j=%d\n",i1,j1);
//printf("bool k=%d\n",k1);

i2=~i;j2=~j;
printf("after ~: \n");

printf("int i=%d,j=%d\n",i2,j2);
//printf("bool k=%d\n",k2);
*/

return 0;
}
