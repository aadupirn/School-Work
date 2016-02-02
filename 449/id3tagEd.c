#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//COE449 ID3 tag editer
//Aadu Pirn, aap75, 3845682

struct id3tag {
	char tag[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[30];
	char comment[28];
	char separator[1];
	char track[1];
	char genre[1];
};

int main(int argc, char *argv[])
{	
	int i;
	for(i = 1; i < argc; i++)
	{
		if(strcmp(argv[i+1], "-title") == 0)
		{
			printf("You asked for the title from %s", argv[i]);
		}
		printf("\nYour arguments are %s", argv[i]);
	}
	
	return 0;
}
