#include <stdio.h>
#include <stdlib.h>

#include <dictionary.h>

#define BUFFER_SIZE 512

int main(int argc, char **argv)
{


  printf("argc=%d\n",argc);
  //printf("argv[0]=%s\n",argv[0]);
  printf("argv[1]=%s\n",argv[1]);

  if(argc!=2){
    printf("only one parameter\n");
    exit(1);
  }

  




/*
  unsigned char buffer[BUFFER_SIZE];
  int in_len=0;
  int i;

  client_len=0;

 
  printf("Start--hex\n");
  for(i=0;i<in_len;i++)
   printf("%02x",buffer[i]);

  printf("\n---char\n");
  for(i=0;i<in_len;i++)
   if(buffer[i]>31&&buffer[i]<127)
    printf("%c",buffer[i]);
   else printf("X");
  printf("\nEnd\n");
 


  int ttt=getDiameterHeaderNum(DiameterHeaderMessageLenth,buffer);
  printf("message length=%d\n",ttt);
  ttt=getDiameterHeaderNum(DiameterHeaderCommandCode,buffer);
  printf("Command Code=%d\n",ttt);
  ttt=getDiameterHeaderNum(DiameterHeaderApplicationID,buffer);
  printf("Application ID=%d\n",ttt);
  
  ttt=findAvpIndexByStr("Origin-Host",buffer);
  printf("Origin-Host index=%d\n",ttt);
  ttt=findAvpIndexByStr("Origin-Realm",buffer);
  printf("Origin-Realm index=%d\n",ttt);

  char ccc[40];
  int lll=getAvpDataStrByNum(buffer,ttt,ccc);
  //printf("Origin-Host = %s\n",ccc);
  printf("Origin-Realm = %s\n",ccc);

*/



 exit(0);
}


