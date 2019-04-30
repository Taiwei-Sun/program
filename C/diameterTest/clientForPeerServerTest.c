#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <dictionary.h>


#define HeadLen 20
#define AvpHeadLen 8
#define OriginHostStr "perry.hss"
#define OriginRealmStr "perry.aa.bb.cc123"
#define VendorIdNum_A 123
#define ProductNameStr "perry production"
#define InbandSecurityIdNum 0
#define FirmwareRevisionNum 1
#define VendorIdNum_B 10415
#define AuthApplicationIdNum 16777251


int Version=0x01;
int MessageLen=0x15c;
int Flags=0x80;
int CommandCode=0x101;
int ApplicationID=0x00;
int HopByHop=0x7d;
int EndToEnd=0xe8e26172;


void intV2CharV(int v1,char *buf,int start,int len)
{
 int i;
 for(i=start;i<len+start;i++){
  int v2=v1;
  v2=v2>>((len+start-i-1)*8);
  v2=v2&0xff;
  buf[i]=(unsigned char)v2;
 }
}


void addMessageHead(char *buf,int len)
{
 intV2CharV(Version,buf,0,1);//version
 intV2CharV(len,buf,1,3);//message length
 intV2CharV(Flags,buf,4,1);//flags
 intV2CharV(CommandCode,buf,5,3);//command code
 intV2CharV(ApplicationID,buf,8,4);//application ID
 intV2CharV(HopByHop,buf,12,4);//hop-by-hop ID
 intV2CharV(EndToEnd,buf,16,4);//end-to-end ID
}

void addAVPStr(char *buf,int code,int flags,int len,char *data, int *indexOri)
{
 int index=*indexOri;
 intV2CharV(code,buf,index,4);
 intV2CharV(flags&0xff,buf,index+4,1);
 intV2CharV(len,buf,index+4+1,3);
 int i;
 for(i=0;i<len-8;i++)
  buf[index+4+1+3+i]=data[i];
  
 for(i=0;i<4-len%4;i++)
  buf[index+len+i]=0;

 *indexOri=index+len+(4-len%4)%4;
}
void addAVPNum32(char *buf,int code,int flags,int len,int data, int *indexOri)
{
 int index=*indexOri;
 intV2CharV(code,buf,index,4);
 intV2CharV(flags&0xff,buf,index+4,1);
 intV2CharV(len,buf,index+4+1,3);
 intV2CharV(data,buf,index+4+1+3,4);

 *indexOri=index+len;
}

int main()
{
 int sockfd;
 int len;
 struct sockaddr_in address;
 int result;

 sockfd=socket(AF_INET, SOCK_STREAM, 0);
 
 address.sin_family=AF_INET;
 address.sin_addr.s_addr=inet_addr("127.0.0.1");
 address.sin_port=htons(3868);

 len=sizeof(address);

 result=connect(sockfd, (struct sockaddr*)&address, len);
 
 if(result==-1)
 {
  perror("oops: client1");
  exit(1);
 }

 char packet[512],buf[128];
 int pLen=HeadLen;
 //int pLen=0; 


 //add AVP
 addAVPStr(packet,OriginHost,0x40,AvpHeadLen+strlen(OriginHostStr),OriginHostStr,&pLen);
 addAVPStr(packet,OriginRealm,0x40,AvpHeadLen+strlen(OriginRealmStr),OriginRealmStr,&pLen);
 addAVPNum32(packet,VendorId,0x40,AvpHeadLen+sizeof(int),VendorIdNum_A,&pLen);
 addAVPStr(packet,ProductName,0x00,AvpHeadLen+strlen(ProductNameStr),ProductNameStr,&pLen);
 addAVPNum32(packet,InbandSecurityId,0x40,AvpHeadLen+sizeof(int),InbandSecurityIdNum,&pLen);
 addAVPNum32(packet,FirmwareRevision,0x00,AvpHeadLen+sizeof(int),FirmwareRevisionNum,&pLen);
 
 int bufLen=0;
 addAVPNum32(buf,VendorId,0x40,AvpHeadLen+sizeof(int),VendorIdNum_B,&bufLen);
 addAVPNum32(buf,AuthApplicationId,0x40,AvpHeadLen+sizeof(int),AuthApplicationIdNum,&bufLen);
 addAVPStr(packet,VendorSpecificApplicationId,0x40,AvpHeadLen+bufLen,buf,&pLen);

 //add diameter head
 addMessageHead(packet,pLen);


 write(sockfd, packet, sizeof(char)*pLen);

 printf("client:  len=%d\n",pLen);
 


 close(sockfd);
 exit(0);

}


