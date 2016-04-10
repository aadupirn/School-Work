//Aadu Pirn
//aap75
//Project 3 cs449

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
	
struct node {
	int size;
	int isFree;
	struct node *next;
	struct node *previous;
};

void *root = NULL;
struct node *current = NULL;

struct node *findNextFree(int size)
{
	struct node *currentNode = NULL;
	if(current)
	{
		currentNode = current;
	}
	else
	{
		currentNode = root;
	}
	struct node *start = currentNode;
	while (!(currentNode -> isFree && currentNode->size >= size))
	{
		if(!currentNode)
		{
			currentNode = root;
		}
		else if(currentNode == start)
		{
			return NULL;
		}
		else
		{
			currentNode = currentNode->next;
		}
	
	}
	return currentNode;
}

struct node *getSpace(int size) 
{
	struct node *newNode;
	newNode = sbrk(0);
	void *space = sbrk(size + sizeof(struct node));

	newNode->size = size;
	newNode->next = NULL;
	newNode->isFree = 0;
	return newNode;
}

void  *my_nextfit_malloc(int size){
	struct node *newNode;
	if(size <= 0)
	{
		return NULL;
	}
	//haven't called yet
	if(!root)
	{
		newNode = getSpace(size);
		root = newNode;
	}
	else//has been called
	{
		newNode = findNextFree(size);
		if(!newNode)
		{
			newNode = getSpace(size);
		}
	}
	return(newNode+1);
}

void my_nextfit_free(void * p)
{
	if(!p)
	{
		return;
	}
	//need to combine nodes now
	struct node *nodePointer = (struct node*)p - 1; //gotta subtract one size of node to get to the address of the node
	while(nodePointer->previous && nodePointer->previous->isFree == 1)
	{
		nodePointer = nodePointer->previous;
		nodePointer->size = nodePointer->size + nodePointer->next->size + sizeof(struct node);
		nodePointer->next = nodePointer->next->next;
	}
	while(nodePointer->next && nodePointer->next->isFree == 1)
	{
		nodePointer = nodePointer->next;
		nodePointer->size = nodePointer->size + nodePointer->previous->size + sizeof(struct node);
		nodePointer->previous = nodePointer->previous->previous;
	}	
	nodePointer->isFree = 1;	
	if(!nodePointer->next)
	{
		if(nodePointer->previous)
		{
			nodePointer->previous->next = NULL;
			printf("HELLO %d\n",(int)(-1)*(int)(nodePointer->size + sizeof(struct node)));
			sbrk((-1)*(nodePointer->size + sizeof(struct node)));
		}
		else
		{
			sbrk((-1)*(nodePointer->size + sizeof(struct node)));
			root = NULL;
			current = NULL;	
		}
	}
}