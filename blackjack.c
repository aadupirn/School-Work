#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//COE449 Blackjack part of Programming assignment 1
//Aadu Pirn, aap75, 3845682

//Function prototypeing
int getRand();
int main()
{
    printf("Welcome to Blackjack!!!\n\nI am your dealer and we will now play!!\n\n\n");
    srand((unsigned)time(NULL));
    
    int continueBool = 1;
    int findAces = 1;
    int findAcesDealer = 1;
    int dealer[20];
    int player[20];
    int dealerCount = 2;
    int dealerSum = 0;
    int playerCount = 2;
    int playerSum = 0;

	int i;
    
    char hitString[10] = "hit";
    char standString[10] = "stand";
    char continueString[10]; 
    
    dealer[0] = getRand();
    dealer[1] = getRand();
    player[0] = getRand();
    player[1] = getRand();
    dealerSum = dealer[0] + dealer[1];
    
    printf("The dealer:\n? + %d\n\n", dealer[1]);
    
    do{
        playerSum = 0;
        printf("You:\n");
        for(i = 0; i < playerCount-1; i++)
        {
            printf("%d + ", player[i]);
            playerSum = playerSum + player[i];
        }
        playerSum = playerSum + player[playerCount-1];
        printf("%d = %d\n", player[playerCount-1], playerSum);
        
        if(playerSum > 21)
        {
            printf("\nBUSTED!\n\n");
            continueBool = 0;
        }
        //player's total is less than or equal to 21, he can hit or stand if he wants too.
        else
        {
            printf("Would you like to \"hit\" or \"stand\"?");
            scanf("%s", continueString);

            //error check for entering hit or stand
            while((strcmp(hitString, continueString) != 0) &&
                 (strcmp(standString, continueString) != 0))
            {
                printf("\nPlease enter \"hit\" or \"stand\"\n\nWould you like to \"hit\" or \"stand\"?");
                scanf("%s", continueString);
            }
            //Player Stands
            if(strcmp(standString, continueString) == 0)
            {
                continueBool = 0;
                while(dealerSum < 17)
                {
                    dealer[dealerCount] = getRand();
                    dealerSum = dealerSum + dealer[dealerCount];
                    dealerCount++;
                    while(dealerSum > 21 && findAcesDealer)
                    {
                        for(i = 0; i < dealerCount; i++)
                        {
                            if(dealer[i] == 11)
                            {
                                dealer[i] = 1;
                                dealerSum = dealerSum - 10;
                                break;
                            }
                            if(i == dealerCount - 1)
                            {
                                findAcesDealer = 0;
                            }
                        }   
                    }
                }
                printf("\nDealer:\n\n");
                for(i = 0; i < dealerCount-1; i++)
                {
                    printf("%d + ", dealer[i]);
                }
                printf("%d = %d\n", dealer[dealerCount-1], dealerSum);
                
                if(dealerSum > 21)
                {
                    printf("\n\nDEALER BUSTS YOU WIN!!\n");
                }
                else if(dealerSum >= playerSum)
                {
                    printf("\nDealer got %d, you got %d.\n\nDEALER WINS...\n", dealerSum, playerSum);
                }
                else
                {
                    printf("You got %d, Dealer got %d.\n\nYOU WIN!!!!!!!\n", playerSum, dealerSum);
                }
            }
            //Player hits
            else
            {
                player[playerCount] = getRand();
                playerSum = playerSum + player[playerCount];
                playerCount++;

                //turn 11's into 1's one at a time until total is under 21.
                while(playerSum > 21 && findAces)
                {
                    for(i = 0; i < playerCount; i++)
                    {
                        if(player[i] == 11)
                        {
                            player[i] = 1;
                            playerSum = playerSum - 10;
                            break;
                        }
                        if(i == playerCount - 1)
                        {
                            findAces = 0;
                        }
                    }
                }

            }
        }
        
    }while(continueBool);
        
    
    return 0;
}

int getRand()
{
    int number = rand()%13+1;
    if(number == 12 || number == 13 || number == 11)
    {
        number = 10;
    }
    if(number == 1)
    {
        number = 11;
    }
    return number;
}
