#include <stdio.h>

extern int add_function(int n1,int n2);

extern int sub_function(int n1,int n2);

int main(void)
{
	int sum = add_function(30,10);
	int sub = sub_function(30,10);
	printf("summation is %d \n",sum);
	printf("substraction is %d \n",sub);
	return 0;
}
