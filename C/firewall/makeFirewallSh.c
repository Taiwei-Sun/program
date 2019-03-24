#include<stdio.h>
#include<stdlib.h>
//#define readfile "ip.txt"
#define writefile "firewall.sh"
#define iptablesOrder "iptables -I INPUT -p tcp --dport 25565 -j ACCEPT -s" 
#define errorMsg "please keyin a text which has ip tables\nip table like:  192.168.1.0-192.168.1.255[Enter]\n\t\t134.100.0.0-134.100.255.255\nex: %s fileName\n"
//20141113
//20141122
//20141124

int maskCount(int ip[2][4])
{
	int mask=0,maskT=0;
	int j;
	for(j=0;j<4;j++)//make mask
	{
		ip[1][j]^=ip[0][j];
		ip[1][j]=~ip[1][j];
		ip[1][j]&=0x00ff;
		int i;
		for(i=0;i<8;i++)//count mask
		{
			int tmp=ip[1][j]<<i;
			tmp&=0x00ff;
			tmp>>=7;
			if(tmp>0&&mask==maskT){mask++;maskT++;}
			else if(tmp>0&&mask<maskT)maskT++;
			else if(tmp==0&&mask==maskT)maskT++;
			//else if(tmp==0&&mask<maskT)
		}
	}
	if(mask!=(maskT-1))ip[1][3]=0x00ff;
	return mask;
}
void readIp(FILE *rfp,int ip[2][4])
{	
	char ch;
	int j;
	for(j=0;j<2;j++)
	{
	int i;
	for(i=0;i<4;i++)
	{
		ip[j][i]=0;	
		int n=0;		
		while(1)//ip.txt>>ip[][]
		{
			if(feof(rfp)){i=4;j=2;break;}
			ch=getc(rfp);
			if((ch<0x30||ch>0x39)&&n==1)break;
			else if(ch<0x30||ch>0x39)continue;
			else n=1;
			
			ip[j][i]*=10;
			ch-=0x30;
			ip[j][i]+=ch;
		}
	}
	}
}


int main(int argc, char* argv[])
{
	if(argc==1)
	{
	printf(errorMsg,argv[0]);
	exit(1);
	}
	FILE *rfp,*wfp;
	char *readfile=argv[1];
	int ip[2][4]={{0,0,0,0},{0,0,0,0}};
	int mask;
	
	if((rfp=fopen(readfile,"r"))==NULL)
	{
		printf("Open read file fail:%s\n",argv[1]);
		exit(1);
	}
	
	if((wfp=fopen(writefile,"w"))==NULL)
	{
		printf("Open write file fail\n");
		exit(1);
	}
	
	fprintf(wfp,"#!/bin/bash\nPATH=/sbin\n");
	while(1)
	{
	readIp(rfp,ip);	
	if(!feof(rfp))
	{
	printf("%d.%d.%d.%d-%d.%d.%d.%d ->",ip[0][0],ip[0][1],ip[0][2],ip[0][3],ip[1][0],ip[1][1],ip[1][2],ip[1][3]);
	mask=maskCount(ip);
	if(ip[1][3]!=0x00ff)
	printf("%d.%d.%d.%d/%d\n",ip[0][0],ip[0][1],ip[0][2],ip[0][3],mask);
	else 
	printf("%d.%d.%d.%d/%d Not entirely correct\n",ip[0][0],ip[0][1],ip[0][2],ip[0][3],mask);

	fprintf(wfp,iptablesOrder);
	fprintf(wfp," %d.%d.%d.%d/%d ",ip[0][0],ip[0][1],ip[0][2],ip[0][3],mask);
	if(ip[1][3]==0x00ff)fprintf(wfp,"#Not entirely correct ");
	fprintf(wfp,"\n");
	}
	else break;	
	}
	fprintf(wfp,"exit 0\n");
	fclose(rfp);
	fclose(wfp);
	return 0;
	
}

