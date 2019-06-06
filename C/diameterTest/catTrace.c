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


  //printf("argc=%d\n",argc);
  //printf("argv[0]=%s\n",argv[0]);
  //printf("argv[1]=%s\n",argv[1]);

  int len;
  char *packetS;
  if(argc!=2){
    char packetS1[1024];
	scanf("%s",packetS1);
	int pLen=strlen(packetS1);
	printf("pLen=%d\n",pLen);
	packetS=malloc(sizeof(char)*(pLen));
	int i;
	for(i=0;i<pLen;i++){
		packetS[i]=packetS1[i];
	}
	
	
  }else{
	  packetS=argv[1];
	  
  }
  
  len=strlen(packetS);
  //len=strlen(TmpStr);
  int bufLen=(len/2)+(len&2);
  //printf("len=%d\nbufLen=%d\n",len,bufLen);
  char buf[bufLen];  

  char2charV(packetS,buf);
  //char2charV(TmpStr,buf);


  initDiameter(buf);
  
  printf("Diameter version=%d\n",diameterHeaderStruct.version);
  printf("Diameter length=%d\n",diameterHeaderStruct.len);
  printf("Diameter flags=%02x\n",diameterHeaderStruct.flags&0xff);
  printf("Diameter command code=%x\n",diameterHeaderStruct.commandCode);
  printf("Diameter application ID=%x\n",diameterHeaderStruct.applicationID);
  printf("Diameter Hop By Hop ID=%x\n",diameterHeaderStruct.hopByHopID);
  printf("Diameter End To End ID=%x\n",diameterHeaderStruct.endToEndID);
  printf("\n");
  
  
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
   printf("--------\n");
  }




 exit(0);
}


