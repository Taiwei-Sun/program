#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
//#include <dictionary.h>


int main()
{
 
 int i=2;
 printf("i=%d ",i);
 i=i<<4;
 printf("i<<4=%d\n",i); 


 exit(0);

}


