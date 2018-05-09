/****************************/
/* Hector Castillo Martinez */
/* CS241                    */
/****************************/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "hashtable.h"

/*creates new hashtable*/
struct HTable* createHtable()
{
  struct HTable* new=(struct HTable*) malloc(sizeof(struct HTable));
  new->size=2000;
  new->n=0;
  return new;
}
/*creaes new hashtable element*/
struct HashElt* createHashElt()
{
  struct HashElt* new=(struct HashElt*) malloc(sizeof(struct HashElt));
  new->url=NULL;
  new->pageRank=(float)1;
  new->token=NULL;
  new->next=NULL;
  return new;
}
/*turns token from HashElt into a number by addding 
 *the ascii values of each char in the token string.
 *treats all char as lowercase*/
int hash(struct HashElt* elt)
{
  int myhash=0;
  int i=0;
  char * string=elt->token;
  while(string[i])
  {
    if(string[i]<97)myhash+=(string[i]+32);/*checks for uppercase*/
    else myhash+=string[i];
    i++;
  }  
  return myhash;
}

/*turns the token from the HashElt into a number by adding the ascii 
 *values of each char in the token string */
void insert(struct HashElt* elt,struct HTable* table)
{
  struct HashElt* af;
  int myhash=hash(elt);
  int index;
  index=(myhash) % (table->size);
  af=table->table[index];
  /*if hash index empty, set it to first*/
  if(af==NULL)
  {
    table->table[index]=elt;
  }
  else if((strcmp(elt->url,af->url)==0) && (strcmp(elt->token,af->token)==0))return;
  else
  {
    while(af->next!=NULL)/*loops until end*/
    {
      if((strcmp(elt->url,af->next->url)==0) && (strcmp(elt->token,af->next->token)==0))return;
      af=af->next;
    }
    af->next=elt;
  }
  table->n++;
}

/*returns a linked list of everything that token hashes to the same
 *value as key*/
struct HashElt* lookup(char* key, struct HTable* table)
{
  int index;
  struct HashElt* temp=createHashElt();
  temp->token=key;
  index=(hash(temp)) % (table->size);
  temp=table->table[index];
  if(temp==NULL)
  {
    printf("Not in table\n");
    return temp;
  }
  else return temp;
}
