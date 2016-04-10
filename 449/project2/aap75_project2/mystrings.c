//Developer: Aadu Pirn (aap75)
//Project 2 CS449 for jmisurda
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[] )
{
	char filename[100];
	strcpy(filename, argv[1]);
	FILE *file;
	file = fopen(filename, "r");
	if(file == NULL)
	{
		printf("Error in opening file please try again with a different filename");
		return(-1);
	}
	char currentC;
	int counter = 0;
	char currentS[1000000];
	do
	{		
		currentC = fgetc(file);
		if((31<currentC) && (currentC<127))
		{
			int len = strlen(currentS);
			currentS[len] = currentC;
			currentS[len+1] = '\0';
			counter++;
		}
		else if(counter>3)
		{
			printf("\n%s", currentS);
			strcpy(currentS, "");
			counter = 0;
		}
		else
		{
			strcpy(currentS, "");
			counter = 0;			
		}		
		if(feof(file))
		{
			if(counter>3)
			{
				printf("\n%s", currentS);
			}
			break;
		}
	}while(1);
	
	fclose(file);
	return 0;
}
