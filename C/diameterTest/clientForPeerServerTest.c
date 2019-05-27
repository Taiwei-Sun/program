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
 addAVPDataStrByCodeStr(packet,"Origin-Host",0x40,AvpHeadLen+strlen(OriginHostStr),OriginHostStr,&pLen);
 addAVPDataStrByCodeStr(packet,"Origin-Realm",0x40,AvpHeadLen+strlen(OriginRealmStr),OriginRealmStr,&pLen);
 addAVPDataNum32ByCodeStr(packet,"Vendor-Id",0x40,AvpHeadLen+sizeof(int),VendorIdNum_A,&pLen);
 addAVPDataStrByCodeStr(packet,"Product-Name",0x00,AvpHeadLen+strlen(ProductNameStr),ProductNameStr,&pLen);
 addAVPDataNum32ByCodeStr(packet,"Inband-Security-Id",0x40,AvpHeadLen+sizeof(int),InbandSecurityIdNum,&pLen);
 addAVPDataNum32ByCodeStr(packet,"Firmware-Revision",0x00,AvpHeadLen+sizeof(int),FirmwareRevisionNum,&pLen);
 
 int bufLen=0;
 addAVPDataNum32ByCodeStr(buf,"Vendor-Id",0x40,AvpHeadLen+sizeof(int),VendorIdNum_B,&bufLen);
 addAVPDataNum32ByCodeStr(buf,"Auth-Application-Id",0x40,AvpHeadLen+sizeof(int),AuthApplicationIdNum,&bufLen);
 addAVPDataStrByCodeStr(packet,"Vendor-Specific-Application-Id",0x40,AvpHeadLen+bufLen,buf,&pLen);

 //add diameter head
 addDiameterHead(packet,pLen,CommandCode,ApplicationID,HopByHop,EndToEnd);


 write(sockfd, packet, sizeof(char)*pLen);

 printf("client:  len=%d\n",pLen);
 


 close(sockfd);
 exit(0);

}


