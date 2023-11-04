#include <stdio.h>
//#include "helloworld.h"
int main(void)
{
	#ifdef x
	printf("x = 5 \n");
	#endif
	#ifdef y
	printf("y = 10 \n");
	#endif
	printf("error \n");

	return 0;
}
