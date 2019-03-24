#include <stdio.h>


struct people {
   int id;
   int age;
   int height;
   int weight;
};

int main()
{
 struct people p;
 struct people p1;
 struct people *p2=&p;
 
 p1.id=1;
 p2->id=2;

 printf("p1.id=%d\n",p1.id);
 printf("p2->id=%d\n",p2->id);
 printf("(*p2).id=%d\n",(*p2).id);

 return 0;
}
