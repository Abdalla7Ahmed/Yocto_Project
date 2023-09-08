#include <stdio.h>
#include "add.h"
extern int add(int x,int y);
int main(void)
{
	int a , b ;
	 printf("Enter a : ");
	scanf("%d",&a);
	printf("Enter b  : ");
	scanf("%d",&b);
	int sum = add(a,b);
	printf("%d + %d = %d \n",a,b,sum);
	return 0;
}
