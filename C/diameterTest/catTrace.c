#include <stdio.h>
#include <stdlib.h>

#include <string.h>
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

  int len=strlen(argv[1]);
  //printf("len=%d\n",len);
  int bufLen=(len/2)+(len&2);
  printf("bufLen=%d\n",bufLen);
  char buf[bufLen];  

  int i;
  for(i=0;i<len;i++){
    int v=argv[1][i]-0x30;
    if(v>=0x0&&v<0x0a){
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    if(v>=0x11&&v<0x17){
      v=v-0x10+0x0a;
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    if(v>=0x31&&v<0x37){
      v=v-0x30+0x0a;
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    printf("buf[%d]=%02x\n",i/2,buf[i/2]);
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


