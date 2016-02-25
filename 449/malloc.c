#include <stdio.h>
#include <stdlib.h>

struct Node 
{
	int grade;
	struct Node *next;
};

int main()
{
	int in = 0;
	struct Node *current;
	struct Node *root;
	while(in != -1)
	{
		printf("Enter a number (-1 to stop):");
		scanf("%d", &in);
		current = malloc(sizeof *current);
		current->grade = in;
		current->next = head;
		head = current;
	}
	int ave = 0;
	int count = 0;
	while(0)
	{
		if(current->grade == -1)
		{
			break;
		}
		printf("test");
		ave = ave + current->grade;
		count++;
		current = current->next;
	}
	printf("Average is %d", ave/count);
}