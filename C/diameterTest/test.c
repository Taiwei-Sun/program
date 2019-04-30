#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <dictionary.h>


int main()
{
 
 
 char buf[50];
 int rr=0;
 rr=getStrByNum(buf,264);
 if(rr!=0)
  printf("getStrByNum(str,264) str=%s\n",buf);
 else
  printf("no find any\n");
 
 rr=getNumByStr("Origin-Realm");
 printf("getNumByStr(\"Origin-Realm\") Num=%d\n",rr);


 exit(0);

}


