
#define AvpHeaderMax 3
#define AvpHeaderLenMax 8
#define AvpHeaderCode 0
#define AvpHeaderFlags 1
#define AvpHeaderLength 2


int AvpHeaderIndexListNum[AvpHeaderMax]={0,4,5};



#define DiameterHeaderMax 7
#define DiameterHeaderLenMax 20
#define DiameterHeaderVersion 0
#define DiameterHeaderMessageLenth 1
#define DiameterHeaderFlags 2
#define DiameterHeaderCommandCode 3
#define DiameterHeaderApplicationID 4
#define DiameterHeaderHopByHop 5
#define DiameterHeaderEndToEnd 6

#define AvpMaxOnPacket 15

int DiameterHeaderIndexListNum[DiameterHeaderMax]={0,1,4,5,8,12,16};


#define AvpMax 49
#define AvpStrMax 40
char AvpListStr[AvpMax][AvpStrMax]={"Acct-Interim-Interval","Accounting-Realtime-Required","Acct-Multi-Session-Id","Accounting-Record-Number","Accounting-Record-Type","Acct-Session-Id","Accounting-Sub-Session-Id","Acct-Application-Id","Auth-Application-Id","Auth-Request-Type","Authorization-Lifetime","Auth-Grace-Period","Auth-Session-State","Re-Auth-Request-Type","Class","Destination-Host","Destination-Realm","Disconnect-Cause","Error-Message","Error-Reporting-Host","Event-Timestamp","Experimental-Result","Experimental-Result-Code","Failed-AVP","Firmware-Revision","Host-IP-Address","Inband-Security-Id","Multi-Round-Time-Out","Origin-Host","Origin-Realm","Origin-State-Id","Product-Name","Proxy-Host","Proxy-Info","Proxy-State","Redirect-Host","Redirect-Host-Usage","Redirect-Max-Cache-Time","Result-Code","Route-Record","Session-Id","Session-Timeout","Session-Binding","Session-Server-Failover","Supported-Vendor-Id","Termination-Cause","User-Name","Vendor-Id","Vendor-Specific-Application-Id"};
int AvpListNum[AvpMax]={85,483,50,485,480,44,287,259,258,274,291,276,277,285,25,293,283,273,281,294,55,297,298,279,267,257,299,272,264,296,278,269,280,284,33,292,261,262,268,282,263,27,270,271,265,295,1,266,260};


int DiameterVersion=0x01;
int DiameterFlags=0x80;
typedef struct AvpData {
  int packetIndex;
  int code;
  char flags;
  int len;
  char *data;
} AvpData;
//AvpData avpIndex[AvpMaxOnPacket];
AvpData *avpIndex;
int avpIndexNum=-1;

//!!!Function


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

int charV2IntV(char *buf,int start,int len)
{
 int i,v=0;
 for(i=start;i<start+len;i++){
	//printf("01 i=%d v=%x\n",i,v);
	v=v<<8;
	//printf("02 i=%d v=%x\n",i,v);
	v=v+(buf[i]&0xff);
	//printf("03 i=%d v=%x\n",i,v);
	//printf("buf[%d]=%x\n",i,buf[i]);
 }
 
 return v;
}
char getAvpCodeStrByNum(char* str,int num){
 char findNum=-1;
 int i,j;
 for(i=0;i<AvpMax;i++){
  if(AvpListNum[i]==num){
   findNum=1;
   for(j=0;j<AvpStrMax;j++)
    str[j]=AvpListStr[i][j];
  }
 }
 return findNum;
}

int getAvpCodeNumByStr(char* str){
 int i,j;
 for(i=0;i<AvpMax;i++){
  j=0;
  while(j<AvpStrMax){
   if(AvpListStr[i][j]!=str[j])j=AvpStrMax;
   else if(AvpListStr[i][j]=='\0')return AvpListNum[i];
   j++;
  }
 }
 return 0;
}


void addDiameterHead(char *buf,int len,int code,int appID,int hhID,int eeID)
{
 intV2CharV(DiameterVersion,buf,0,1);//version
 intV2CharV(len,buf,1,3);//message length
 intV2CharV(DiameterFlags,buf,4,1);//flags
 intV2CharV(code,buf,5,3);//command code
 intV2CharV(appID,buf,8,4);//application ID
 intV2CharV(hhID,buf,12,4);//hop-by-hop ID
 intV2CharV(eeID,buf,16,4);//end-to-end ID
}

void addAVPDataStr(char *buf,int code,int flags,int len,char *data, int *indexOri)
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
void addAVPDataStrByCodeStr(char *buf,char *codeStr,int flags,int len,char *data, int *indexOri)
{
	addAVPDataStr(buf,getAvpCodeNumByStr(codeStr),flags,len,data, indexOri);
}

void addAVPDataNum32(char *buf,int code,int flags,int len,int data, int *indexOri)
{
 int index=*indexOri;
 intV2CharV(code,buf,index,4);
 intV2CharV(flags&0xff,buf,index+4,1);
 intV2CharV(len,buf,index+4+1,3);
 intV2CharV(data,buf,index+4+1+3,4);

 *indexOri=index+len;
}
void addAVPDataNum32ByCodeStr(char *buf,char *codeStr,int flags,int len,int data, int *indexOri)
{
	addAVPDataNum32(buf,getAvpCodeNumByStr(codeStr),flags,len,data, indexOri);
}




int getDiameterHeaderNum(int DH,char* buf){
	
	int len;
	int index=DiameterHeaderIndexListNum[DH];
	if(DH+1<DiameterHeaderMax)
		len=DiameterHeaderIndexListNum[DH+1]-index;
	else len=DiameterHeaderLenMax-index;
	
	//printf("charV2IntV(buf,index,len)=charV2IntV(buf,%d,%d)\n",index,len);
	return charV2IntV(buf,index,len);
}

int getAvpHeaderNum(int AH,char* buf,int start){
	int len;
	int index=AvpHeaderIndexListNum[AH];
	
	if(AH+1<AvpHeaderMax)
		len=AvpHeaderIndexListNum[AH+1]-index;
	else len=AvpHeaderLenMax-index;
	
	//printf("charV2IntV(buf,start+index,len)=charV2IntV(buf,%d+%d,%d)\n",start,index,len);
	return charV2IntV(buf,start+index,len);
}

void getAvpDataStrByPacketIndexNum(char* packet,int index,int len,char* buf){

	int i;
	for(i=0;i<len;i++)
		buf[i]=packet[index+AvpHeaderLenMax+i];	
	
}



int checkAvpIndex(int index)
{
        if(avpIndexNum==-1){
                printf("no init AVP Index\n");
                return -1;
        }
        if(index>=avpIndexNum){
                printf("Wrong AVP Index number\n");
                return -1;
        }

	return 0;
}



void getAvpDataStrByIndex(int index,char *buf)
{
	if(checkAvpIndex(index)==-1)return;

	int i;
	for(i=0;i<avpIndex[index].len;i++)
		buf[i]=avpIndex[index].data[i];
  
}

int getAvpNum(char *packet)
{
  int i=0;
  int p=20;
  int len=getDiameterHeaderNum(DiameterHeaderMessageLenth,packet);
  while(p<len){

   int aLen=getAvpHeaderNum(AvpHeaderLength,packet,p);
   aLen=aLen+(4-aLen%4)%4;//for padding
   p=p+aLen;
   i++;
  }
  return i;
}

void initAvpIndex(char *packet)
{
	//printf("0\n");
	int pos=20;
	int len=getDiameterHeaderNum(DiameterHeaderMessageLenth,packet);
	//printf("1 len=%d\n",len);
	avpIndexNum=getAvpNum(packet);
	printf("avpIndexNum=%d\n",avpIndexNum);
	avpIndex=malloc(sizeof(AvpData)*avpIndexNum);
	

	int p=pos;
	int i=0;
	//printf("2\n");
	while(p<len){
		//printf("i=%d ",i);
		avpIndex[i].packetIndex=p;
		avpIndex[i].code=getAvpHeaderNum(AvpHeaderCode,packet,p);
		avpIndex[i].flags=getAvpHeaderNum(AvpHeaderFlags,packet,p)&0xff;
		avpIndex[i].len=getAvpHeaderNum(AvpHeaderLength,packet,p);
		avpIndex[i].data=malloc(sizeof(char)*(avpIndex[i].len-8));
		getAvpDataStrByPacketIndexNum(packet,p,avpIndex[i].len,avpIndex[i].data);
		
		
		int avpLen=avpIndex[i].len+(4-avpIndex[i].len%4)%4;//for padding
		p=p+avpLen;
		i++;
		
	}
	
	//printf("\n3\n");
	
}

//---old
int AcctInterimInterval=85;int AccountingRealtimeRequired=483;int AcctMultiSessionId=50;int AccountingRecordNumber=485;int AccountingRecordType=480;int AcctSessionId=44;int AccountingSubSessionId=287;int AcctApplicationId=259;int AuthApplicationId=258;int AuthRequestType=274;int AuthorizationLifetime=291;int AuthGracePeriod=276;int AuthSessionState=277;int ReAuthRequestType=285;int Class=25;int DestinationHost=293;int DestinationRealm=283;int DisconnectCause=273;int ErrorMessage=281;int ErrorReportingHost=294;int EventTimestamp=55;int ExperimentalResult=297;int ExperimentalResultCode=298;int FailedAVP=279;int FirmwareRevision=267;int HostIPAddress=257;int InbandSecurityId=299;int MultiRoundTimeOut=272;int OriginHost=264;int OriginRealm=296;int OriginStateId=278;int ProductName=269;int ProxyHost=280;int ProxyInfo=284;int ProxyState=33;int RedirectHost=292;int RedirectHostUsage=261;int RedirectMaxCacheTime=262;int ResultCode=268;int RouteRecord=282;int SessionId=263;int SessionTimeout=27;int SessionBinding=270;int SessionServerFailover=271;int SupportedVendorId=265;int TerminationCause=295;int UserName=1;int VendorId=266;int VendorSpecificApplicationId=260;
