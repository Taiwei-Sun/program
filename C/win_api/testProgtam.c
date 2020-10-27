#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int a=7,b=9,n=7;
	
	while(n!=0){
		printf(" one number:");
		scanf("%d",&n);
		if(n>0)a=n;
		if(n<0)b=-n;
		printf("&a=%p a=%d &b=%p b=%d\n",&a,a,&b,b);
	}


	return 0;
}
