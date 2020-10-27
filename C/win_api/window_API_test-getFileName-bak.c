#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#include <string.h>

#define UNKNOW_PROCESS_NAME "<unknown>"
#define DEBUG_READ_LEN 1
#define DEBUG_READ_PROCESS_NAME_LEN 0
#define PROCESS_NAME_MAX 50
#define ADDRESSES_BUFFER 2048

typedef struct Node Node;

struct Node{
	SIZE_T data;
	Node *next;
};

void addNode(Node **head,SIZE_T data){
	Node *newNode=(Node*)malloc(sizeof(Node));
	newNode->data=data;
	newNode->next=0;
	
	if(*head){
		newNode->next=*head;
		
	}
	*head=newNode;
}

SIZE_T getValueOfNodeByIndex(Node *head,int index){
	int i=0;
	while(head!=0){
		if(i==index){
			return head->data;
		}
		i++;
	}
	return 0;
}

void printNode(Node *head){
	while(head!=0){
		printf("%p\n",head->data);
		head=head->next;
	}
}


//void GetProcessNameByID(DWORD processID,TCHAR* szProcessName)
void GetProcessNameByID(DWORD processID,TCHAR szProcessNameTmp[PROCESS_NAME_MAX])
{
	HANDLE hProcess=OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	TCHAR szProcessName[MAX_PATH] = TEXT(UNKNOW_PROCESS_NAME);
	
	
	// Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
            int len=GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
			#if DEBUG_READ_PROCESS_NAME_LEN !=0
			//printf("sizeof(szProcessName)/sizeof(TCHAR)=%d/%d=%d\n",sizeof(szProcessName),sizeof(TCHAR),sizeof(szProcessName)/sizeof(TCHAR));
			printf("len=%d strlen(szProcessName)=%d\n",len,strlen(szProcessName));
			#endif
        }
		else{
			#if DEBUG_READ_PROCESS_NAME_LEN !=0
			printf("EnumProcessModules=False strlen(szProcessName)=%d\n",strlen(szProcessName));
			#endif
		}
    }
	else{
		#if DEBUG_READ_PROCESS_NAME_LEN !=0
		printf("handle=Null strlen(szProcessName)=%d\n",strlen(szProcessName));
		#endif
	}
	strcpy(szProcessNameTmp,szProcessName);
    
    
    // Release the handle to the process.

    CloseHandle( hProcess );
}

SIZE_T returnLenFromProcessByID(DWORD pid,SIZE_T loc,char buf[],int len) {
	HANDLE pHandle=NULL;
	LPCVOID lpMem=(LPCVOID)loc;
	SIZE_T totalRead;
	
	
	
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (pHandle == NULL) {
		return '\0'; //do something with your error
	}
	
	if(ReadProcessMemory(pHandle, lpMem, buf, sizeof(char)*len, &totalRead))// && totalRead==sizeof(char)*len)
	{
		//printf("address=0x%x  value=0x%08x\n",lpMem,buf);//for buf of DWORD
		#if DEBUG_READ_LEN!=0
		printf("succeeds totalRead=%d base addr=%p\n",totalRead,lpMem);
		#endif
	}
	else {
		#if DEBUG_READ_LEN!=0
		printf("fails totalRead=%d base addr=%p\n",totalRead,lpMem);
		#endif
		int errorTmp=GetLastError();
		
		if(errorTmp!=299)printf("read process memory error: %d\n",errorTmp);
		
		#if DEBUG_READ_LEN!=0
		if(errorTmp==6)printf("address=0x%x\n",lpMem);
		#endif
		
		if(errorTmp==998){
			if(totalRead!=0){
				printf("error 998 and totalRead=%p\n",totalRead);
				CloseHandle(pHandle);
				return totalRead;
			}
			else {
				CloseHandle(pHandle);
				return -1;
			}
		}
		
		CloseHandle(pHandle);
		return 0;
	}
	
	CloseHandle(pHandle);
	return totalRead;
}

void firstSearch4byteFromMemory(int processID,int targetInt,Node **head,SIZE_T *searchAddrNum){
	char buf[ADDRESSES_BUFFER];
	SIZE_T i=0,j,totalRead=0;
	Node *newHead=0;
	
	totalRead=returnLenFromProcessByID(processID,i,buf,ADDRESSES_BUFFER);
	
	while(totalRead !=-1){
		printf("totalRead=%d\n",totalRead);
		if(totalRead==0){
			totalRead=returnLenFromProcessByID(processID,i,buf,ADDRESSES_BUFFER);
			i+=ADDRESSES_BUFFER-3;
			continue;
		}
		for(j=0;j<totalRead-3;j++){
			if(buf[j]==targetInt&&buf[j+1]==0&&buf[j+2]==0&&buf[j+3]==0){
				addNode(&newHead,i+j);
				(*searchAddrNum)++;
				//printf("\nfind %d, total=%d address=%p\n",targetInt,*searchAddrNum,i+j);
			}
			
		}
		
		totalRead=returnLenFromProcessByID(processID,i,buf,ADDRESSES_BUFFER);
		i+=totalRead-3;//3 mean If the data we are looking for is split
	}
	
	
}

void search4byteFromMemory(int processID,int targetInt,Node **head,SIZE_T *searchAddrNum){
	if((*searchAddrNum)==0)return;
	SIZE_T i,len,index,totalRead;
	char buf[4];
	Node *newHead=0;
	
	len=*searchAddrNum;
	
	*searchAddrNum=0;
	
	printf("\n0%%");
	for ( i = 0; i < len; i++ )
    {
		index=getValueOfNodeByIndex(*head,i);
		
		totalRead=returnLenFromProcessByID(processID,index,buf,4);
		if(buf[0]==targetInt&&buf[1]==0&&buf[2]==0&&buf[3]==0){
			//printf("\nfind %d, total=%d address=%p\n",targetInt,*searchAddrNum+1,i);
			addNode(&newHead,index);
			(*searchAddrNum)++;
			//printf("\n*searchAddrNum=%d\n",*searchAddrNum);
		}
		if(len/100!=0&&i%(len/100)==0)printf("\r%5.2f%%",(float)i*100/len);
		else if(len/10!=0&&i%(len/10)==0)printf("\r%5.2f%%",(float)i*100/len);
		
	}
	printf("\r100.00%%\n");
	*head=newHead;
}



//_tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );

int main( void )
{
	
	TCHAR szProcessName[PROCESS_NAME_MAX];
	//printf("MAX_PATH=%d\n",MAX_PATH);
	//TCHAR processName[10]=TEXT("notepad");
	TCHAR* processName=(TCHAR*)TEXT("testProgtam.exe");
	DWORD processID=0;
	
    // Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
	
	
	
	

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }
	
	//printf("%d\n",strcmp(processName,"notepAd"));

    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);
	TCHAR processesName[cProcesses][PROCESS_NAME_MAX];
	int processesID[cProcesses];
	

    // Print the name and process identifier for each process.
    for ( i = 0; i < cProcesses; i++ )
    {
        if( aProcesses[i] != 0 )
        {
			
            //PrintProcessNameAndID( aProcesses[i] );
			printf("%d. ",i);
			GetProcessNameByID(aProcesses[i],szProcessName);
			strcpy(processesName[i],szProcessName);
			//printf("%d. process name=\"%s\"\n",i,szProcessName);//error!!!
			printf("process name=\"%s\"\n",processesName[i]);
			
			
			processesID[i]=aProcesses[i];
			
			/*
			if(strcmp(processName,szProcessName)==0)
			{
				//printf("%s id=<%d>\n",processName,aProcesses[i]);
				processID=aProcesses[i];
				//break;
			}
			*/
        }
    }
	
	int processNum;
	printf("please, select a number:");
	scanf("%d",&processNum);
	printf("you select %d, process id=%d process name=%s\n",processNum,processesID[processNum],processesName[processNum]);
	processID=processesID[processNum];
	
	
	
	
	int searchInt=9;
	SIZE_T searchAddrNum=0;
	Node *searchNode=0;
	int selectNum=-1;
	
	
	printf("Please, input a number you want search:");
	scanf("%d",&searchInt);
	firstSearch4byteFromMemory(processID,searchInt,&searchNode,&searchAddrNum);
	printf("searchAddrNum=%d\n",searchAddrNum);
	
	printNode(searchNode);
	//printf("getValueOfNodeByIndex(searchNode,3)=%p\n",getValueOfNodeByIndex(searchNode,3));
	
	
	/*
	while(selectNum!=0){
		printf("1. search again\n2. show all searched addresses\n3. write memory\n4. search value by address\nPlease, select one:");
		scanf("%d",&selectNum);
		if(selectNum==1){
			printf("Please, input a number you want search:");
			scanf("%d",&searchInt);
			search4byteFromMemory(maxMemorySize,processID,searchInt,&searchNode,&searchAddrNum);
			printf("searchAddrNum=%d\n",searchAddrNum);
		}
		
		if(selectNum==4){
			printf("Please, input a address you want to show:");
			scanf("%x",&searchAddrNum);
			printf("address=%p value=%d\n",searchAddrNum,returnByteFromProcessByID(processID,searchAddrNum));
			printf("address=%p value=%d\n",searchAddrNum+1,returnByteFromProcessByID(processID,searchAddrNum+1));
			printf("address=%p value=%d\n",searchAddrNum+2,returnByteFromProcessByID(processID,searchAddrNum+2));
			printf("address=%p value=%d\n",searchAddrNum+3,returnByteFromProcessByID(processID,searchAddrNum+3));
			
		}
		
	}
	*/
	
	
	
	/*
	printf("0%%");
	for ( i = 0; i < maxMemorySize; i++ )
    {
		//printf("value=0x%x\n",i,returnByteFromProcessByID(processID,i));
		buf=returnByteFromProcessByID(processID,i);
		if(buf==searchInt&&returnByteFromProcessByID(processID,i-1)==0&&returnByteFromProcessByID(processID,i-2)==0&&returnByteFromProcessByID(processID,i-3)==0){
			searchAddresses[searchAddrNum]=i;
			//printf("\nfind %d, total=%d address=%p\n",searchInt,searchAddrNum+1,i);
			searchAddrNum++;
		}
		if(i%1000==0)printf("\r%5.2f%%",(float)i*100/maxMemorySize);
	}
	printf("\r100.00%\n");
	*/
	
	
	/*
	LPDWORD idTest;
	//HANDLE hProcess=GetActiveWindow();
	HWND hProcess=GetForegroundWindow();
	GetWindowThreadProcessId(hProcess,idTest);
	PrintProcessNameByHandle(hProcess);
	printf(" id=%d\n",idTest);
	*/
	
	
    return 0;
}
