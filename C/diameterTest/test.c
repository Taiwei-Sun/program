#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>


int main()
{
 unsigned char *c=&(unsigned char){0xaa};
 int i;
 
 //char cc=0x03;

 for(i=0;i<3;i++)
  printf("c[%d]=%x\n",i,c[i]); 
 
 //printf("cc=%02x\n",cc);
 exit(0);

}


