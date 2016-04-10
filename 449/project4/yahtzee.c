//Aadu Pirn
//aap75
//Project 3 cs449
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void printDice(int roll[]);
int cmpfunc (const void * a, const void * b);

int main()
{
	int roundCount = 0;
	int rollCount = 0;
	time_t t;
	srand((unsigned) time(&t));
	printf("\n\n\nWelcome to Yahtzee!! Today you are just going to play by yourself.\n\n\
As a reminder here are the categories you can choose to put points in and the scoring for each set:\n\n\
--------UPPER SECTION--------\n\
Ones\n\
Twos\n\
Threes\t\tPoints in the Upper secion are worth the\n\
Fours\t\tsum of the dice with the appropriate value\n\
Fives\n\
Sixes\n\
--------LOWER SECTION--------\n\
Three of a Kind.....Sum of all 5 dice\n\
Four of a Kind......25\n\
Full House..........30\n\
Small Straight......40\n\
Large Straight......50\n\
Yahtzee.............50\n\
Chance..............Sum of all 5 dice\n\
------------------------------\n\
A score of more than 63 points in the upper section will result in a ponus 35 points.\n\n\
Here is your first roll:\n");
	int points[13];
	int i;
	for(i = 0; i < 13; i++)
	{
		points[i] = -1;
	}
	while(roundCount < 13)
	{
		int roll[5];
		char rerollString[100];
		roll[0] = (rand()%6) + 1;
		roll[1] = (rand()%6) + 1;
		roll[2] = (rand()%6) + 1;
		roll[3] = (rand()%6) + 1;
		roll[4] = (rand()%6) + 1;
		rollCount++;
		while(rollCount < 3)
		{
			printDice(roll);
			printf("\nPlease enter the number of the die/dice you want to reroll (Enter zero to skip rerolls):");
			int rerolls[5];
			for(i = 0; i < 5; i++)
			{
				rerolls[i] = -1;
			}
			int num;
			char buffer[1000];
			fgets(buffer, sizeof buffer, stdin);
			const char *p = buffer;
			int n;
			int count = 0;
			while (sscanf(p, "%d%n", &num, &n) == 1)  //getting numbers that are rerolled
			{
				p += n;
				rerolls[count] = num;
				count++;
			} 
			
			if(rerolls[0] == 0)
			{
					break;
			}
			for(i = 0; i < 5; i++)
			{
				if(rerolls[i] != -1)
				{
					roll[rerolls[i]-1] = (rand()%6) +1;
				}
				else
				{
					break;
				}
			}
			rollCount++;
			printf("%d", rollCount);
		}
		rollCount = 0;
		qsort(roll, 5, sizeof(int), cmpfunc);
		int threeKind = 0;
		int fourKind = 0;
		int yahtzee = 0;
		int smallS = 0;
		int largeS = 0;
		int kindCounter = 1;
		int straightCounter = 1;
		for(i = 1; i < 5; i++)
		{
			if(roll[i] == roll[i-1])
			{
				kindCounter++;
			}
			if(roll[i] = (roll[i-1] + 1))
			{
				straightCounter++;
			}
		}
		
		roundCount++;
	}
	
	

	return 0;
	
   

}

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void printDice(int roll[])
{
	printf("%d  %d  %d  %d  %d", roll[0], roll[1], roll[2], roll[3], roll[4]);
}