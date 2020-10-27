#include <stdio.h>

int sum(int x, int y)
{
    return x+y;
}

int sub(int x, int y)
{
    return x-y;
}

int mult(int x, int y)
{
    return x*y;
}

int div(int x, int y)
{
    return x/y;
}

int main( )
{
    int (*fp)(int, int);
    fp = sum;
    int s = fp(15, 5);
    printf("Sum is %d\n", s);
	
	fp = sub;
    s = fp(15, 5);
    printf("sub is %d\n", s);
	
	fp = mult;
    s = fp(15, 5);
    printf("mult is %d\n", s);
	
	fp = div;
    s = fp(15, 5);
    printf("div is %d\n", s);

    return 0;
}



