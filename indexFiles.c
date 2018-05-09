/**************************/
/*Hector Castillo Martinez*/
/*CS 241                  */
/**************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "indexFiles.h"

/* Reads in a file and stores each element in the hashtable.
 *Filename to be open passed from graph and need to be in directory.
 *done word by word using isalpha to get rid of all not alphabetical
 *characers*/
void readInFile(char* fn,struct HTable* table,float pr)
{
  struct HashElt* temp;
  FILE* file=fopen(fn,"r");
  int c;
  int index=0;
  char* word=malloc(sizeof(char)*20);
  if(file==NULL)
  {
    perror("Error in readInFile");
    return;
  }
  while((c=fgetc(file))!=EOF)
  {
    if((c>=65 && c<=90) || (c>=97 && c<=122))/*alphabetical char*/
    {
      c=tolower(c);
      word[index]=c;
      index++;
    }
    if(isalpha(c)==0 && word[0]!=0)/*words needs to be built*/
    {
      temp=createHashElt();
      temp->url=fn;
      temp->token=word;
      temp->pageRank=pr;
      insert(temp,table);
      index=0;
      word=(char*)calloc(20,sizeof(char));
    }
  }
  fclose(file);
}

