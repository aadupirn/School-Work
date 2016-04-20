//Aadu Pirn 
//CS449 449
//Project 5

//Uses strtok()
//use fgets()

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//constants for parsing strings.
const char LEFT[2] = "<";
const char RIGHT[2] = ">";
const char SPACE[2] = " ";
const char LPAR[2] = "(";
const char RPAR[2] = ")";
const char ORCH[2] = "|";
const char ANDCH[2] = "&";
const char DASH[2] = "-";

char pwd[1024]; //string to get present working directory. Going to start at the location where this executable resides.
char command[1024]; //string to stare desired command in.
	
char *token;
	

int main()
{
	//Print greeting message
	printf("\nInitializing your very own Unix based Shell...\nYou honestly should probably use whatever shell you started this from... but whatever.\nDeveloped by Aadu Pirn\nDeveloped for CS0449 at The University of Pittsburgh\n\n");

	
	
	int run = 1;
	while(run)
	{
		getcwd(pwd, sizeof(pwd));
		printf("SHELL:%s$ ", pwd);
		fgets(command, 1024, stdin);
		token = strtok(command, SPACE);
		printf("\n\nCommand: %s\n\n", command);
		if(strcmp(token, "cd") == 0)
		{
			
		}
		else if(strcmp(token, "exit") == 0)
		{
			
		}
		else if(strcmp(token, "ls") == 0)
		{
			
		}
		else if(strcmp(token, "pico") == 0)
		{
			
		}
		else if(strcmp(token, "pwd") == 0)
		{
			printf("You asked for the present working directory!!");
			token = strtok(NULL, SPACE);
			printf("the argument of the command was: %s", token);
		}
		else if(strcmp(token, "ls") == 0)
		{
			
		}
		else if(strcmp(token, "wc") == 0)
		{
			
		}
		else if(strcmp(token, "ps") == 0)
		{
			
		}
		else if(strcmp(token, "time") == 0)
		{
			
		}
		else if(strcmp(token, "cd") == 0)
		{
			
		}
		run = 0;
	}
	
	
	
	printf("\n\nExiting this awesome shell. Good luck in the world without me.\n\n");
	return 0;
}