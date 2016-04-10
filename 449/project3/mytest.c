//Aadu Pirn
//aap75
//Project 3 cs449
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"
#include <string.h>


//replace malloc here with the appropriate version of mymalloc
#define MALLOC my_nextfit_malloc
//replace free here with the appropriate version of myfree
#define FREE my_nextfit_free

int main()
{
	char *str;

	/* Initial memory allocation */
	str = (char *) MALLOC(15);
	strcpy(str, "project3Test");
	printf("String = %s,  Address = %u\n", str, *str);

	FREE(str);

	int *ptr_one;

	ptr_one = (int *)MALLOC(sizeof(int));

	if (ptr_one == 0)
	{
		printf("ERROR\n");
		return 1;
	}

	*ptr_one = 25;
	printf("%d\n", *ptr_one);

	FREE(ptr_one);

	return 0;
	
   

}