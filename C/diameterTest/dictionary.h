//20190524 add AVP index
#define AvpHeaderMax 3
#define AvpHeaderLenMax 8
#define AvpHeaderCode 0
#define AvpHeaderFlags 1
#define AvpHeaderLength 2

int AvpHeaderIndexListNum[AvpHeaderMax]={0,4,5};


//20190523 diameter header index and Avp index
//20190430 AVP code list
#define DiameterHeaderMax 7
#define DiameterHeaderLenMax 20
#define DiameterHeaderVersion 0
#define DiameterHeaderMessageLenth 1
#define DiameterHeaderFlags 2
#define DiameterHeaderCommandCode 3
#define DiameterHeaderApplicationID 4
#define DiameterHeaderHopByHop 5
#define DiameterHeaderEndToEnd 6

int DiameterHeaderIndexListNum[DiameterHeaderMax]={0,1,4,5,8,12,16};


#define AvpMax 49
#define AvpStrMax 40
char AvpListStr[AvpMax][AvpStrMax]={
"Acct-Interim-Interval",
"Accounting-Realtime-Required",
"Acct-Multi-Session-Id",
"Accounting-Record-Number",
"Accounting-Record-Type",
"Acct-Session-Id",
"Accounting-Sub-Session-Id",
"Acct-Application-Id",
"Auth-Application-Id",
"Auth-Request-Type",
"Authorization-Lifetime",
"Auth-Grace-Period",
"Auth-Session-State",
"Re-Auth-Request-Type",
"Class",
"Destination-Host",
"Destination-Realm",
"Disconnect-Cause",
"Error-Message",
"Error-Reporting-Host",
"Event-Timestamp",
"Experimental-Result",
"Experimental-Result-Code",
"Failed-AVP",
"Firmware-Revision",
"Host-IP-Address",
"Inband-Security-Id",
"Multi-Round-Time-Out",
"Origin-Host",
"Origin-Realm",
"Origin-State-Id",
"Product-Name",
"Proxy-Host",
"Proxy-Info",
"Proxy-State",
"Redirect-Host",
"Redirect-Host-Usage",
"Redirect-Max-Cache-Time",
"Result-Code",
"Route-Record",
"Session-Id",
"Session-Timeout",
"Session-Binding",
"Session-Server-Failover",
"Supported-Vendor-Id",
"Termination-Cause",
"User-Name",
"Vendor-Id",
"Vendor-Specific-Application-Id"};
int AvpListNum[AvpMax]={85,483,50,485,480,44,287,259,258,274,291,276,277,285,25,293,283,273,281,294,55,297,298,279,267,257,299,272,264,296,278,269,280,284,33,292,261,262,268,282,263,27,270,271,265,295,1,266,260};


char getAvpStrByNum(char* str,int num){
 char findNum=0;
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

int getAvpNumByStr(char* str){
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

int charV2IntV(char *buf,int start,int len)
{
 int i,v=0;
 for(i=start;i<start+len;i++){
	v=v<<8;
	v=v+(buf[i]&0xff);
	//printf("03 i=%d v=%x\n--\n",i,v);
	//printf("buf[%d]=%x\n",i,buf[i]&0xff);
 }
 
 return v;
}

int getDiameterHeaderNum(int DH,char* buf){
	
	int len;
	int index=DiameterHeaderIndexListNum[DH];
	if(DH+1<DiameterHeaderMax)
		len=DiameterHeaderIndexListNum[DH+1]-index;
	else len=DiameterHeaderLenMax-index;
	
	return charV2IntV(buf,index,len);
}

int getAvpHeaderNum(int AH,char* buf,int start){
	int len;
	int index=AvpHeaderIndexListNum[AH];
	
	if(AH+1<AvpHeaderMax)
		len=AvpHeaderIndexListNum[AH+1]-index;
	else len=AvpHeaderLenMax-index;

	return charV2IntV(buf,start+index,len);
}

int getAvpDataStrByNum(char* packet,int index,char* buf){
	int len=getAvpHeaderNum(AvpHeaderLength,packet,index);
	

	int i;
	for(i=0;i<len;i++)
		buf[i]=packet[index+AvpHeaderLenMax+i];	
	
	return len;
}





int findAvpIndexByStr(char* str,char* buf){
	int index=20;
	int len=getDiameterHeaderNum(DiameterHeaderMessageLenth,buf);
	int avpCode=getAvpNumByStr(str);

	int i=index;
	while(i<len){
		printf("getAvpHeaderNum(AvpHeaderCode,buf,%d)=%d\n",i,getAvpHeaderNum(AvpHeaderCode,buf,i));
		if(avpCode==getAvpHeaderNum(AvpHeaderCode,buf,i))return i;
		int avpLen=getAvpHeaderNum(AvpHeaderLength,buf,i);
		avpLen=avpLen+(4-avpLen%4)%4;//for padding
		i=i+avpLen;
	}

	return -1;
	
}


//---old
int AcctInterimInterval=85;int AccountingRealtimeRequired=483;int AcctMultiSessionId=50;int AccountingRecordNumber=485;int AccountingRecordType=480;int AcctSessionId=44;int AccountingSubSessionId=287;int AcctApplicationId=259;int AuthApplicationId=258;int AuthRequestType=274;int AuthorizationLifetime=291;int AuthGracePeriod=276;int AuthSessionState=277;int ReAuthRequestType=285;int Class=25;int DestinationHost=293;int DestinationRealm=283;int DisconnectCause=273;int ErrorMessage=281;int ErrorReportingHost=294;int EventTimestamp=55;int ExperimentalResult=297;int ExperimentalResultCode=298;int FailedAVP=279;int FirmwareRevision=267;int HostIPAddress=257;int InbandSecurityId=299;int MultiRoundTimeOut=272;int OriginHost=264;int OriginRealm=296;int OriginStateId=278;int ProductName=269;int ProxyHost=280;int ProxyInfo=284;int ProxyState=33;int RedirectHost=292;int RedirectHostUsage=261;int RedirectMaxCacheTime=262;int ResultCode=268;int RouteRecord=282;int SessionId=263;int SessionTimeout=27;int SessionBinding=270;int SessionServerFailover=271;int SupportedVendorId=265;int TerminationCause=295;int UserName=1;int VendorId=266;int VendorSpecificApplicationId=260;
