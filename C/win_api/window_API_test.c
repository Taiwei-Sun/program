/*
https://docs.microsoft.com/en-us/windows/win32/procthread/process-security-and-access-rights
https://docs.microsoft.com/en-us/windows/win32/secauthz/access-rights-and-access-masks
https://docs.microsoft.com/en-us/windows/win32/secbp/changing-privileges-in-a-token
https://docs.microsoft.com/en-us/windows/win32/secauthz/enabling-and-disabling-privileges-in-c--


*/


#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#include <string.h>

#include <conio.h>

#define UNKNOW_PROCESS_NAME "<unknown>"
#define DEBUG_READ_LEN 0
#define DEBUG_READ_PROCESS_NAME_LEN 0
#define PROCESS_NAME_MAX 50
#define PROCESS_NUMBER_MAX 1024
#define ADDRESSES_BUFFER 0x10000
#define ADDRESSES_HEX_DIGITS_LEN 16
#define SAVE_FILE_NAME "address.txt"

//#pragma comment( lib, "psapi.lib" )



DWORD WINAPI monitorUserInput(LPVOID data) {
	char *ch=(char *)data;
	printf("If you want to STOP, please type 'q'\n");
	while((*ch=getch())!='q'){
		printf("Thread ch=%c\n",*ch);
	}
	return 0;
}


typedef struct Node{
	LPCVOID data;
	char comment[10];
	struct Node *next;
}Node;

void addNode(Node **head,LPCVOID data){
	Node *newNode=(Node*)malloc(sizeof(Node));
	newNode->data=data;
	newNode->next=0;
	
	if(*head){
		newNode->next=*head;
		
	}
	*head=newNode;
}

void freeNode(Node *head){
	Node *pre=0;
	while(head!=0){
		pre=head;
		head=head->next;
		free(pre);
	}
}

LPCVOID getValueOfNodeByIndex(Node *p,int index){
	int i=0;
	while(p!=0){
		if(i==index){
			return p->data;
		}
		p=p->next;
		i++;
	}
	return 0;
}

LPCVOID getNextValueOfNode(Node **p){
	LPCVOID tmp=0;
	if(*p!=0){
		tmp=(*p)->data;
		*p=(*p)->next;
	}
	return tmp;
}

void printNode(Node *head){
	int i=0;
	while(head!=0){
		printf("%p\n",head->data);
		i++;
		head=head->next;
	}
}

void printNodeLine(Node *head){
	int i=0;
	while(head!=0){
		printf("%d. %p\n",i,head->data);
		i++;
		head=head->next;
	}
}

void printNodeLineToFile(Node *head,FILE *f){
	int i=0;
	while(head!=0){
		fprintf(f,"%d. %p\n",i,head->data);
		i++;
		head=head->next;
	}
}

void intTo4Char(int i,char ch[4]){
	
	ch[0]=i%256;
	i=i>>8;
	ch[1]=i%256;
	i=i>>8;
	ch[2]=i%256;
	i=i>>8;
	ch[3]=i%256;
	
}

//void GetProcessNameByID(DWORD processID,TCHAR* szProcessName)
void GetProcessNameByID(DWORD processID,TCHAR szProcessNameTmp[PROCESS_NAME_MAX])
{
	//| PROCESS_VM_READ
	HANDLE hProcess=OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
	strcpy(szProcessNameTmp,UNKNOW_PROCESS_NAME);
	
	// Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
        {
			int len=GetModuleBaseName( hProcess, hMod, szProcessNameTmp, PROCESS_NAME_MAX/sizeof(TCHAR) );
#if DEBUG_READ_PROCESS_NAME_LEN !=0
			//printf("sizeof(szProcessNameTmp)/sizeof(TCHAR)=%d/%d=%d\n",sizeof(szProcessNameTmp),sizeof(TCHAR),sizeof(szProcessNameTmp)/sizeof(TCHAR));
			printf("len=%d strlen(szProcessNameTmp)=%d\n",len,strlen(szProcessNameTmp));
#endif
        }
		else{
#if DEBUG_READ_PROCESS_NAME_LEN !=0
			printf("EnumProcessModules=False strlen(szProcessNameTmp)=%d\n",strlen(szProcessNameTmp));
#endif
		}
    }
	else{
#if DEBUG_READ_PROCESS_NAME_LEN !=0
		printf("handle=Null strlen(szProcessNameTmp)=%d\n",strlen(szProcessNameTmp));
#endif
	}
    
    
    // Release the handle to the process.

    CloseHandle( hProcess );
}

void firstSearchIntFromMemory(DWORD pid,int targetInt,Node **head,SIZE_T *searchAddrNum,SIZE_T max,SIZE_T min){
	HANDLE pHandle=NULL;
	LPCVOID lpMem;
	MEMORY_BASIC_INFORMATION info;
	SIZE_T i;
	Node *newHead=0;
	printf("start search VALUE(%d)\n",targetInt);
	
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (pHandle == NULL) {
		return; //do something with your error
	}
	
    
    for ( lpMem = 0;VirtualQueryEx(pHandle, lpMem, &info, sizeof(info)) == sizeof(info); lpMem = (BYTE *)lpMem + info.RegionSize ) 
    {
        
		//if(lpMem!=info.BaseAddress)printf("\nlpMem=%p info.BaseAddress=%p info.AllocationBase=%p\n",lpMem,info.BaseAddress,info.AllocationBase);
		
		if( max>min && (lpMem>(LPCVOID)max || (BYTE *)lpMem + info.RegionSize <(LPCVOID)min) ){
			continue;
		}
		
		printf("\rNow searched ADDRESS is %p",lpMem);
        if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE) ) 
        {
			
            SIZE_T bytes_read;
            //char buf[info.RegionSize];
			char buf[ADDRESSES_BUFFER];
			char tmpBuf[4];
			SIZE_T wantReadByte,totalReadByte=info.RegionSize,base=0;
			
			
			while(totalReadByte>0){
				if(ADDRESSES_BUFFER>=totalReadByte){
					wantReadByte=totalReadByte;
					totalReadByte=0;
				}
				else{
					wantReadByte=ADDRESSES_BUFFER;
					totalReadByte-=ADDRESSES_BUFFER-3;// -3 mean if search Int be split 
				}
				ReadProcessMemory(pHandle, (BYTE *)lpMem+base, buf, wantReadByte, &bytes_read);
				if(bytes_read<3)bytes_read=3;//bytes_read(SIZE_T) most be positive
				for(i=0;i<bytes_read-3;i++){
					intTo4Char(targetInt,tmpBuf);
					if(buf[i]==tmpBuf[0]&&buf[i+1]==tmpBuf[1]&&buf[i+2]==tmpBuf[2]&&buf[i+3]==tmpBuf[3]){
						//printf("buf[i]=%d targetInt=%d\n",buf[i],targetInt);
						addNode(&newHead,(BYTE *)lpMem+base+i);
						(*searchAddrNum)++;
					}
				
				}
				base+=ADDRESSES_BUFFER-3;// -3 mean if search Int be split
			}
			
			
			//printf("info.RegionSize=%x bytes_read=%x searchAddrNum=%d\n",info.RegionSize,bytes_read,*searchAddrNum);
			//free(buf); //error
            
        }
    }
	printf("\n");
	
	freeNode(*head);
	*head=newHead;
	CloseHandle(pHandle);
}

int writeIntToProcessByID(DWORD pid,SIZE_T loc,int buf) {
	HANDLE pHandle=NULL;
	LPVOID lpMem=(LPVOID)loc;
	
	
	
	pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if (pHandle == NULL) {
		return '\0'; //do something with your error
	}
	
	if(WriteProcessMemory(pHandle, lpMem, &buf, sizeof(buf), NULL)) 
	{ 
		//** If we write the memory successfully, we let the user know 
		printf("WriteProcessMemory is a success! address=%p\n",lpMem); 
		//** Alerts user that it has written 
	} 
	else 
	{ 
		//** If we cant write the memory to our process, we notify 
		//** the user with the following error message; 
		printf("Error cannot WriteProcessMemory! address=%p\n",lpMem);
		return '\0';
	} 
	
	CloseHandle(pHandle);
	return 1;
}

SIZE_T returnLenFromProcessByID(DWORD pid,LPCVOID lpMem,char buf[],int len) {
	HANDLE pHandle=NULL;
	//LPCVOID lpMem=(LPCVOID)loc;
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


void search4byteFromMemory(int processID,int targetInt,Node **head,SIZE_T *searchAddrNum){
	if((*searchAddrNum)==0)return;
	LPCVOID index;
	int i,len;
	char buf[4],tmpBuf[4];
	Node *newHead=0,*p=*head;
	
	len=*searchAddrNum;
	
	*searchAddrNum=0;
	
	printf("\n0%%");
	for ( i = 0; i < len; i++ )
    {
		index=getNextValueOfNode(&p);
		returnLenFromProcessByID(processID,index,buf,4);
		//printf("targetInt=%d buf[0]==%d address=%p\n",targetInt,buf[0],index);
		intTo4Char(targetInt,tmpBuf);
		if(buf[0]==tmpBuf[0]&&buf[1]==tmpBuf[1]&&buf[2]==tmpBuf[2]&&buf[3]==tmpBuf[3]){
			//printf("targetInt=%d buf[0]=%d buf[1]=%d buf[2]=%d buf[3]=%d \n",targetInt,buf[0],buf[1],buf[2],buf[3]);
			addNode(&newHead,index);
			(*searchAddrNum)++;
		}
		if(len/1000!=0&&i%(len/1000)==0)printf("\r%5.2f%%",(float)i*100/len);
		else if(len/100!=0&&i%(len/100)==0)printf("\r%5.2f%%",(float)i*100/len);
		else if(len/10!=0&&i%(len/10)==0)printf("\r%5.2f%%",(float)i*100/len);
		
	}
	printf("\r100.00%%\n");
	
	freeNode(*head);
	*head=newHead;
}

void printMenu(const char *menu[]){
	int i=0;
	while(menu[i]!=0){
		printf("%2d.  %s\n",i,menu[i]);
		i++;
	}
}

SIZE_T getSizetByUserInput(char *addr){
	char ch;
	SIZE_T st=0;
	int i=0;
	//printf("%s\n",addr);
	
	while(addr[i]!='\0'){
		ch=addr[i];
		//printf("%c",addr[i]);
		st*=0x10;
		
		if(ch>='0'&&ch<='9'){
			st+=ch-'0';
		}
		else if(ch>='a'&&ch<='f'){
			st+=ch-'a'+10;
		}
		else if(ch>='A'&&ch<='F'){
			st+=ch-'A'+10;
		}else {
			return 0;
		}
		i++;
		//printf("st=%p\n",st);
	}
	//printf("\n");
	return st;
}

SIZE_T getSize_tFromInput(const char *msg){
	
	char hexBuf[ADDRESSES_HEX_DIGITS_LEN]={};
	printf(msg);
	scanf("%s",hexBuf);
	//printf("%s\n",hexBuf);
	SIZE_T tmpAddr=getSizetByUserInput(hexBuf);
	
	return tmpAddr;
}






//_tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );
int main( void )
{
	
	TCHAR szProcessName[PROCESS_NAME_MAX];
	DWORD processID=0;
	
	const char *menuStr[]={"reset searched address","search again","auto search for const value","show all searched addresses",
					"write memory","search value by address","hold value of address","add address to stack","save searched result",
					"add taged","set addresss range","quit",0};
	//int menuLen=sizeof(menuStr)/sizeof(menuStr[0]);
	
    
    DWORD aProcesses[PROCESS_NUMBER_MAX], cbNeeded, cProcesses;
    unsigned int i;
	
	
	// Get the list of process identifiers.
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }



    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
	
	TCHAR processesName[cProcesses][PROCESS_NAME_MAX];
	int processesID[cProcesses];
	printf("Processes total is %d\n",cProcesses);

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
			
        }
    }
	
	
	int processNum;
	printf("please, select a number for process:");
	scanf("%d",&processNum);
	if(processNum==0){//direct access by PID
		printf("please, select a number for process ID:");
		scanf("%d",&processNum);
		printf("you select process id=%d \n",processNum);
		processID=processNum;
	}
	else {
		printf("you select %d, process id=%d process name=%s\n",processNum,processesID[processNum],processesName[processNum]);
		processID=processesID[processNum];
	}
	
	
	
	
	
	int searchInt=9;
	SIZE_T searchAddrNum=0;
	Node *searchNode=0;
	int selectNum=0;
	int selectAddr;
	char userType;
	HANDLE threadTmp;
	SIZE_T address_max=0,address_min=0;
	
	
	
	while(selectNum!=-1){
		
		printMenu(menuStr);
		printf("Please, select one:");
		scanf("%d",&selectNum);
		const char *selectStr=menuStr[selectNum];
		
		
		if(strcmp("reset searched address",menuStr[selectNum])==0){
			searchAddrNum=0;
			searchNode=0;
			printf("Please, input a VALUE you want first search:");
			scanf("%d",&searchInt);
			firstSearchIntFromMemory(processID,searchInt,&searchNode,&searchAddrNum,address_max,address_min);
			printf("The number of searched addresses found is %d\n",searchAddrNum);
		}
		
		if(strcmp("set addresss range",menuStr[selectNum])==0){
			
			address_max=getSize_tFromInput("please input a max address:");
			address_min=getSize_tFromInput("please input a min address:");
			
		}
		
		
		if(strcmp("search again",selectStr)==0){
			printf("Please, input a VALUE you want search:");
			scanf("%d",&searchInt);
			search4byteFromMemory(processID,searchInt,&searchNode,&searchAddrNum);
			printf("The number of searched addresses found is %d\n",searchAddrNum);
			if(searchAddrNum<5)printNode(searchNode);
		}
		
		if(strcmp("show all searched addresses",selectStr)==0){
			printNode(searchNode);
		}
		
		if(strcmp("write memory",selectStr)==0){
			printf("\n");
			printNodeLine(searchNode);
			printf("Please, select a ADDRESS you want to modify:");
			scanf("%d",&selectAddr);
			printf("Please, input a VALUE you want modify:");
			scanf("%d",&searchInt);
			
			writeIntToProcessByID(processID,(SIZE_T)getValueOfNodeByIndex(searchNode,selectAddr),searchInt);
		}
		
		if(strcmp("search value by address",selectStr)==0){
			/*
			char hexBuf[ADDRESSES_HEX_DIGITS_LEN]={};
			printf("Please, input a ADDRESS(%p~%p) you want to show:",0,INT_MAX);
			scanf("%s",hexBuf);
			//printf("%s\n",hexBuf);
			*/
			SIZE_T tmpAddr=getSize_tFromInput("Please, input a ADDRESS you want to show:");
			char bbuf[4]={};
			
			returnLenFromProcessByID(processID,(LPCVOID)tmpAddr,bbuf,4);
			printf("address=%p value=%d\n",tmpAddr,bbuf[0]);
			printf("address=%p value=%d\n",tmpAddr+1,bbuf[1]);
			printf("address=%p value=%d\n",tmpAddr+2,bbuf[2]);
			printf("address=%p value=%d\n",tmpAddr+3,bbuf[3]);
			
		}
		
		if(strcmp("hold value of address",selectStr)==0){
			printf("\n");
			printNodeLine(searchNode);
			printf("Please, select a ADDRESS you want to keep there value:");
			scanf("%d",&selectAddr);
			printf("Please, input a VALUE you want keep:");
			scanf("%d",&searchInt);
			userType='0';
			threadTmp=0;
			threadTmp = CreateThread(NULL, 0, monitorUserInput, (LPVOID)&userType, 0, NULL);
			while(userType!='q'&&threadTmp){
				writeIntToProcessByID(processID,(SIZE_T)getValueOfNodeByIndex(searchNode,selectAddr),searchInt);
				Sleep(1000);
			}
		}
		
		if(strcmp("auto search for const value",selectStr)==0){
			printf("Please, input a VALUE you want search:");
			scanf("%d",&searchInt);
			userType='0';
			threadTmp=0;
			threadTmp = CreateThread(NULL, 0, monitorUserInput, (LPVOID)&userType, 0, NULL);
			while(userType!='q'&&threadTmp){
				search4byteFromMemory(processID,searchInt,&searchNode,&searchAddrNum);
				printf("The number of searched addresses found is %d\n",searchAddrNum);
				Sleep(1000);
			}
		}
		if(strcmp("add address to stack",selectStr)==0){
			char hexBuf[ADDRESSES_HEX_DIGITS_LEN]={};
			printf("Please, input a ADDRESS you want to store:");
			scanf("%s",hexBuf);
			//printf("%s\n",hexBuf);
			SIZE_T tmpAddr=getSizetByUserInput(hexBuf);
			addNode(&searchNode,(LPCVOID)tmpAddr);
			printf("add address(%p) ok\n",tmpAddr);
			
		}
		
		if(strcmp("save searched result",selectStr)==0){
			FILE *tmpfile=fopen(SAVE_FILE_NAME, "w+");
			
			printNodeLineToFile(searchNode,tmpfile);
			fclose(tmpfile);
			
		}
		
		
		if(strcmp("quit",selectStr)==0){
			selectNum=-1;
		}
		
	}
	
	
	
    return 0;
}
