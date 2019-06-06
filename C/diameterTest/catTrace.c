#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <dictionary.h>

#define BUFFER_SIZE 1024
#define TmpStr "0100015c80000101000000000000007de8e2617200000108400000126665702d702d736374700000000001284000001a7472616666697873797374656d732e636f6d00000000010a4000000c00006bdb0000010d00000031463520547261666669782053797374656d7320436f6e74726f6c20506c616e652046756e6374696f6e0000000000012b4000000c000000000000010b0000000c0000000100000104400000200000010a4000000c000028af000001024000000c0100002300000104400000200000010a4000000c000028af000001024000000c0100003300000104400000200000010a4000000c000028af000001034000000c0000000300000104400000200000010a4000000c000028af000001024000000c0000000400000104400000200000010a4000000c000028af000001024000000c0100001400000104400000200000010a4000000c000028af000001024000000c01000016"


void char2charV(char *str,char *buf){
  int len=strlen(str);
  int i;
  for(i=0;i<len;i++){
    int v=str[i];
    if(v>=0x30&&v<0x3a){
      v=v-0x30;
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    if(v>=0x41&&v<0x47){
      v=v-0x41+0x0a;
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    if(v>=0x61&&v<0x67){
      v=v-0x61+0x0a;
      if(i%2==0)buf[i/2]=0;
      else buf[i/2]*=0x10;
      buf[i/2]+=v;
    }
    //if(i%2!=0)printf("buf[%d]=%02x\n",i/2,buf[i/2]);
    //printf("i=%d ",i);
  }
  printf("\n");
}

int main(int argc, char **argv)
{


  /*printf("argc=%d\n",argc);
  //printf("argv[0]=%s\n",argv[0]);
  printf("argv[1]=%s\n",argv[1]);

  if(argc!=2){
    printf("only one parameter\n");
    exit(1);
  }*/

  int len=strlen(TmpStr);
  //int len=strlen(argv[1]);
  int bufLen=(len/2)+(len&2);
  printf("len=%d\nbufLen=%d\n",len,bufLen);
  char buf[bufLen];  

  char2charV(TmpStr,buf);



  initAvpIndex(buf);
  int i;
  for(i=0;i<avpIndexNum;i++){
   char tmpS[512];
   printf("AVP index=%d\n",i);
   printf("code=%08x\n",avpIndex[i].code);
   getAvpCodeStrByNum(tmpS,avpIndex[i].code);
   printf("code name=\"%s\"\n",tmpS);
   printf("flags=%02x\n",avpIndex[i].flags);
   printf("len=%06x\n",avpIndex[i].len);
   getAvpDataStrByIndex(i,tmpS);
   printf("data=\"%s\"\n",tmpS);
   printf("--------\n\n");
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


