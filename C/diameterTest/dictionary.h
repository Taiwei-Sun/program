
//AVP code
//20190430 update
#define DictionaryMax 49
#define DictionaryStrMax 40
char DictionaryListStr[DictionaryMax][DictionaryStrMax]={
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
int DictionaryListNum[DictionaryMax]={85,483,50,485,480,44,287,259,258,274,291,276,277,285,25,293,283,273,281,294,55,297,298,279,267,257,299,272,264,296,278,269,280,284,33,292,261,262,268,282,263,27,270,271,265,295,1,266,260};

char getAvpStrByNum(char* str,int num){
 char findNum=0;
 int i,j;
 for(i=0;i<DictionaryMax;i++){
  if(DictionaryListNum[i]==num){
   findNum=1;
   for(j=0;j<DictionaryStrMax;j++)
    str[j]=DictionaryListStr[i][j];
  }
 }
 return findNum;
}

int getAvpNumByStr(char* str){
 int i,j;
 for(i=0;i<DictionaryMax;i++){
  j=0;
  while(j<DictionaryStrMax){
   if(DictionaryListStr[i][j]!=str[j])j=DictionaryStrMax;
   else if(DictionaryListStr[i][j]=='\0')return DictionaryListNum[i];
   j++;
  }
 }
 return 0;
}


//---old
int AcctInterimInterval=85;
int AccountingRealtimeRequired=483;
int AcctMultiSessionId=50;
int AccountingRecordNumber=485;
int AccountingRecordType=480;
int AcctSessionId=44;
int AccountingSubSessionId=287;
int AcctApplicationId=259;
int AuthApplicationId=258;
int AuthRequestType=274;
int AuthorizationLifetime=291;
int AuthGracePeriod=276;
int AuthSessionState=277;
int ReAuthRequestType=285;
int Class=25;
int DestinationHost=293;
int DestinationRealm=283;
int DisconnectCause=273;
int ErrorMessage=281;
int ErrorReportingHost=294;
int EventTimestamp=55;
int ExperimentalResult=297;
int ExperimentalResultCode=298;
int FailedAVP=279;
int FirmwareRevision=267;
int HostIPAddress=257;
int InbandSecurityId=299;
int MultiRoundTimeOut=272;
int OriginHost=264;
int OriginRealm=296;
int OriginStateId=278;
int ProductName=269;
int ProxyHost=280;
int ProxyInfo=284;
int ProxyState=33;
int RedirectHost=292;
int RedirectHostUsage=261;
int RedirectMaxCacheTime=262;
int ResultCode=268;
int RouteRecord=282;
int SessionId=263;
int SessionTimeout=27;
int SessionBinding=270;
int SessionServerFailover=271;
int SupportedVendorId=265;
int TerminationCause=295;
int UserName=1;
int VendorId=266;
int VendorSpecificApplicationId=260;
