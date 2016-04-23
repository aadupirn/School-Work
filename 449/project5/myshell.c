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
const char delims[100] = "\t \n()<>|&;";

char pwd[1024]; //string to get present working directory. Going to start at the location where this executable resides.
char command[1024]; //string to stare desired command in.
	
char *token;

int execute(char **arg)
{
	pid_t id;
	int status;
	id = fork();
	
	if(id < 0)
	{
		printf("ERROR: forking process failure\n");
	}
	else if(id == 0)
	{
		if(execvp(*arg, arg) < 0)
		{
			printf("ERROR: falure to execute command\n");
		}
	}
	else
	{
		while(wait(&status) != id)
		{}
	}
}

int main()
{
	//Print greeting message
	printf("\nInitializing your very own Unix based Shell...\nYou honestly should probably use whatever shell you started this from... but whatever.\nDeveloped by Aadu Pirn\nDeveloped for CS0449 at The University of Pittsburgh\n\n");
	char *arg[64];
	
	
	int run = 1;
	while(run)
	{
		getcwd(pwd, sizeof(pwd));
		printf("SHELL:%s $ ", pwd);
		fgets(command, 1024, stdin);
		token = strtok(command, delims);
		
		if(strcmp(token, "cd") == 0)
		{
			token = strtok(NULL, delims);
			if(chdir(token)<0)
			{
				printf("ERROR: invalid directory\n");
			}
		}
		else if(strcmp(token, "exit") == 0)
		{
			run = 0;
		}
		else
		{
			if(strcmp(token, "time") == 0)
			{
				
			}
			int counter = 0;
			while(token != NULL)
			{
				arg[counter] = token;
				counter++;
				token = strtok(NULL, delims);
			}
			arg[counter] = NULL;
			execute(arg);
		}
	}
	
	
	
	printf("\n\nExiting this awesome shell. Good luck in the world without me.\n\n");
	exit(0);
	return(0);
}