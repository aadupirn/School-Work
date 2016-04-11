//Aadu Pirn
//aap75
//Project 3 cs449

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void printDice(int roll[]);
int compfunc (const void * a, const void * b);

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
A score of more than 63 points in the upper section will result in a ponus 35 points.\n\n");
	int points[13];
	int i;
	for(i = 0; i < 13; i++)
	{
		points[i] = -1;
	}
	while(roundCount < 13)
	{
		printf("Here is your roll:\n");
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
			printf("\n");
			printDice(roll);
			printf("\nPlease enter the number of dice you want to reroll: ");
			
			int rerolls[5];
			for(i = 0; i < 5; i++)
			{
				rerolls[i] = -1;
			}
			int numR;
			scanf("%d", &numR);
			
			
			if(rerolls[0] == 0)
			{
					break;
			}
			for(i = 0; i < numR; i++)
			{
				printf("\nEnter the index of a die you want to reroll: ");
				scanf("%d", &rerolls[i]);
			}
			for(i = 0; i < numR; i++)
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
		}
		printf("\nYour final roll is: ");
		printDice(roll);
		rollCount = 0;
		qsort(roll, 5, sizeof(int), compfunc);
		int threeKind = 0;
		int fourKind = 0;
		int yahtzee = 0;
		int smallS = 0;
		int largeS = 0;
		int fullH = 0;
		int sameCounter = 1;
		int straightCounter = 1;
		int bestSameCard = 1;
		int bestStraight = 1;
		int sameCard = 0;
		for(i = 1; i < 5; i++)
		{
			if(roll[i] == roll[i-1])
			{
				sameCounter++;
				if(sameCounter > bestSameCard)
				{
					bestSameCard = sameCounter;
					sameCard = roll[i];
				}
			}
			else
			{
				sameCounter = 1;
			}
			if(roll[i] == (roll[i-1] + 1))
			{
				straightCounter++;
				if(straightCounter > bestStraight)
				{
					bestStraight = straightCounter;
				}
			}
			else
			{
				straightCounter = 1;
			}
		}
		if(((roll[0] == roll[1]) && (roll[2] == roll[3]) && (roll[3] == roll[4])) ||
		((roll[0] == roll[1]) && (roll[1] == roll[2]) && (roll[3] == roll[4]))
		)
		{
			fullH = 1;
		}
		printf("\nSorted it looks like this: ");
		printDice(roll);
		if(sameCounter>2)
		{
			threeKind = 1;
			if(sameCounter>3)
			{
				fourKind = 1;
				if(sameCounter>4)
				{
					yahtzee = 1;
				}
			}
		}
		if(straightCounter>3)
		{
			smallS = 1;
			if(straightCounter>4)
			{
				largeS = 1;
			}
		}
		printf("\n\nHere is your board so far. You can apply your dice to any section without points.\n\
Applying your dice to a category where it would score 0 will score 0 for that category.\n\
If the current point total for a section is -1 then you have not scored there yet and still need to fill it.\n\
--------UPPER SECTION--------\n\
1) Ones................... %d\n\
2) Twos................... %d\n\
3) Threes................. %d\n\
4) Fours.................. %d\n\
5) Fives.................. %d\n\
6) Sixes.................. %d\n\
--------LOWER SECTION--------\n\
7) Three of a Kind........ %d\n\
8) Four of a Kind......... %d\n\
9) Full House............. %d\n\
10) Small Straight........ %d\n\
11) Large Straight........ %d\n\
12) Yahtzee............... %d\n\
13) Chance................ %d\n\
------------------------------\n\
Enter a number 1-13 to choose where to apply this roll: \n", points[0], points[1], points[2], points[3], points[4], points[5], points[6], points[7], points[8], points[9], points[10], points[11], points[12]);
		int pointApplyChoice;
		scanf("%d", &pointApplyChoice);
		int pointSum = 0;
		switch(pointApplyChoice)
		{
			case(1):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 1)
					{
						pointSum = pointSum + 1;
					}
				}
				points[0] = pointSum;
				break;
			case(2):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 2)
					{
						pointSum = pointSum + 2;
					}
				}
				points[1] = pointSum;
				break;
			case(3):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 3)
					{
						pointSum = pointSum + 3;
					}
				}
				points[2] = pointSum;
				break;
			case(4):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 4)
					{
						pointSum = pointSum + 4;
					}
				}
				points[3] = pointSum;
				break;
			case(5):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 5)
					{
						pointSum = pointSum + 5;
					}
				}
				points[4] = pointSum;
				break;
			case(6):
				for(i = 0; i < 5; i++)
				{
					if(roll[i] == 6)
					{
						pointSum = pointSum + 6;
					}
				}
				points[5] = pointSum;
				break;
			case(7):
				for(i = 0; i < 5; i++)
				{
					pointSum = pointSum + roll[i];	
				}
				points[6] = pointSum;
				break;
			case(8):
				for(i = 0; i < 5; i++)
				{
					pointSum = pointSum + roll[i];	
				}
				points[7] = pointSum;
				break;
			case(9):
				if(fullH)
				{
					points[8] = 25;
				}
				else
				{
					points[8] = 0;
				}
				break;
			case(10):
				if(smallS)
				{
					points[9] = 30;
				}
				else
				{
					points[9] = 00;
				}
				break;
			case(11):
				if(largeS)
				{
					points[10] = 40;
				}
				else
				{
					points[10] = 0;
				}
				break;
			case(12):
				if(yahtzee)
				{
					points[11] = 50;
				}
				else
				{
					points[11] = 0;
				}
				break;
			case(13):
				for(i = 0; i < 5; i++)
				{
					pointSum = pointSum + roll[i];	
				}
				points[12] = pointSum;
				break;
		}		
		roundCount++;
	}
	int upperSum = 0;
	int totalSum = 0;
	int bonus = 0;
	for(i = 0; i < 6; i++)
	{
		upperSum = upperSum + points[i];
	}
	if(upperSum > 62)
	{
		bonus = 35;
		totalSum = upperSum + 35;
	}
	else
	{
		totalSum = upperSum;
	}
	for(i = 0; i < 13; i++)
	{
		totalSum = totalSum + points[i];
	}
	
	printf("\n\nHere are your final scores!!\n\
--------UPPER SECTION--------\n\
1) Ones................... %d\n\
2) Twos................... %d\n\
3) Threes................. %d\n\
4) Fours.................. %d\n\
5) Fives.................. %d\n\
6) Sixes.................. %d\n\
--------LOWER SECTION--------\n\
7) Three of a Kind........ %d\n\
8) Four of a Kind......... %d\n\
9) Full House............. %d\n\
10) Small Straight........ %d\n\
11) Large Straight........ %d\n\
12) Yahtzee............... %d\n\
13) Chance................ %d\n\
------------------------------\n\
Bonus: %d\n\
Total: %d\n", points[0], points[1], points[2], points[3], points[4], points[5], points[6], points[7], points[8], points[9], points[10], points[11], points[12], bonus, totalSum);
	
	
	

	return 0;
}

int compfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

void printDice(int roll[])
{
	printf("%d  %d  %d  %d  %d", roll[0], roll[1], roll[2], roll[3], roll[4]);
}