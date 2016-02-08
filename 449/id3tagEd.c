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
	char year[4];
	char comment[28];
	char separator[1];
	char track[1];
	char genre[1];
};

int main(int argc, char *argv[])
{	
	int i;
	int entryError = 0;
	FILE *filePointer;
	filePointer = fopen(argv[1], "r+");
	struct id3tag tag;
	struct id3tag *tagP = &tag;
	//There cannot be an odd number of command line arguments.
	if(argc%2 != 0)
	{
		printf("\nPlease enter a valid command for the id3tag editor\n\n");
	}
	//file does not exist
	else if(!filePointer)
	{
		//they want to create a new tag
		if(argc > 2)
		{		
			//create empty tag
			strncpy(tag.tag, "", 3);
			strncpy(tag.title, "", 30);
			strncpy(tag.artist, "", 30);
			strncpy(tag.album, "", 30);
			strncpy(tag.year, "", 4);
			strncpy(tag.comment, "", 28);
			strncpy(tag.separator, "", 1);			
			strncpy(tag.track, "", 1);
			strncpy(tag.genre, "", 1);
			
			char field[30];
			char value[30];			
			for(i = 2; i < argc; i = i + 2)
			{					
				strncpy(field, argv[i], 30);
				strncpy(value, argv[i+1], 30);				
				if(strcmp(field, "-title") == 0)
				{						
					strncpy(tag.title, value, 30);
				}
				else if(strcmp(field, "-artist") == 0)
				{
					strncpy(tag.artist, value, 30);
				}
				else if(strcmp(field, "-album") == 0)
				{
					strncpy(tag.album, value, 30);
				}
				else if(strcmp(field, "-year") == 0)
				{
					strncpy(tag.year, value, 4);
				}
				else if(strcmp(field, "-comment") == 0)
				{
					strncpy(tag.comment, value, 28);
				}
				else if(strcmp(field, "-track") == 0)
				{
					strncpy(tag.track, value, 1);
				}
				else
				{
					entryError = 1;
					break;
				}
			}
			
			if(!entryError)
			{
				filePointer = fopen(argv[1], "w");
				fwrite(tagP, 128, 1, filePointer);							
				printf("\nCreated new tag with specified contents\n\n");
			}
			else
			{
				printf("\"%s\" is not an available field.\n\n", field);
			}
			
		}
		//there is no tag at the specified filename to be read
		else
		{
			printf("That file does not exist\n\n");
		}
	}
	//file does exist
	else
	{
		//they want to change the tag
		if(argc > 2)
		{
			fread(tagP, 128, 1, filePointer);
			char field[20];
			char value[30];			
			for(i = 2; i < argc; i = i + 2)
			{
				strcpy(field, argv[i]);
				strcpy(value, argv[i+1]);
				if(strcmp(field, "-title") == 0)
				{
					strncpy(tag.title, value, 30);
				}
				else if(strcmp(field, "-artist") == 0)
				{
					strncpy(tag.artist, value, 30);
				}
				else if(strcmp(field, "-album") == 0)
				{
					strncpy(tag.album, value, 30);
				}
				else if(strcmp(field, "-year") == 0)
				{
					strncpy(tag.year, value, 4);
				}
				else if(strcmp(field, "-comment") == 0)
				{
					strncpy(tag.comment, value, 28);
				}
				else if(strcmp(field, "-track") == 0)
				{
					strncpy(tag.track, value, 1);
				}
				else
				{
					entryError = 1;
					break;
				}
			}
			
			if(!entryError)
			{
				filePointer = fopen(argv[1], "w");
				fwrite(tagP, 128, 1, filePointer);
				printf("\nEdited Existing tag with specified contents\n\n");
			}
			else
			{
				printf("\"%s\" is not an available field.\n\n", field);
			}
			
		}
		//they want to read the tag contents
		else
		{
			fread(tagP, 128, 1, filePointer);
			printf("\ntitle: %.30s\nartist: %.30s\nalbum: %.30s\nyear: %.4s\ncomment: %.28s\ntrack: %.1s\n\n", tag.title, tag.artist, tag.album, tag.year, tag.comment, tag.track);
			
		}	
	}
	
	//only close if there are an even number of command line arguments
	if(argc % 2 == 0 && argc > 2)
	{
		fclose(filePointer);
	}
	
	return 0;
}
