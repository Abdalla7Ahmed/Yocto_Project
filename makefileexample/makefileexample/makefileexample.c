#include <stdio.h>

extern int add(int a,int b);

extern int sub(int a,int b);

extern int mul(int a,int b);

int main(void)
{
	int sum_V = add(30,10);
	int sub_v = sub(30,10);
	int mul_v = mul(30,10);
	printf("summation is %d \n",sum_V);
	printf("subtraction is %d \n",sub_v);
	printf("multiplication is %d \n",mul_v);
	return 0;
}
