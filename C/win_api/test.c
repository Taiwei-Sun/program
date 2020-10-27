#include <stdio.h>

void sum(void) 
{
	static int i=0;
	printf("%d\n",i);
	i++;
	
}

int main() 
{ 
	sum();
	sum();
	sum();
	sum();
	return 0; 
}
