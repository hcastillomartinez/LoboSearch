/***************************/
/* Hector Castillo Martinez*/
/* CS 241                  */
/***************************/
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "graph.h"
#include "hashtable.h"
#include "indexFiles.h"
#include "graphUtils.c"

struct top
{
  char* url;
  float pr;
  struct top* next;
};

void printlist(struct top* list);
void stringLwr(char* s);
struct top* createtop();  
struct top* search(struct HashElt* temp, char* key);
struct top* sort(struct top* head,char* url,float pr);

int main(int argc, char* argv[])
{
  char* graph=argv[1];
  char* key=argv[2];
  struct graph* g=createGraph();
  struct HTable* t=createHtable();
  struct HashElt* temp=createHashElt();
  struct top* top=createtop();
  struct node* curr;
  
  if(argc!=3)
  {
    printf("Invalid arguments\n");
    return 0;
  }  
  g=readGraph(graph);
  calculatePageRank(g);
  curr=g->firstNode;
  while(curr!=NULL)/*passes files names from graph to be read*/
  {
    readInFile(curr->url,t,curr->pagerank);
    curr=curr->next;
  }
  temp=lookup(key,t);
  if(temp==NULL)return 0;/*nothing hashed to what user it looking for*/
  top=search(temp,key);
  if(top->url==NULL)/*search not found in the HashElt list*/
  {
    printf("Not in table\n");
    return 0;
  }
  else printlist(top);  
  return 0;
}
/*prints the top 5 urls from hashelt that contain
 *what the user has specified*/
void printlist(struct top* top)
{
  int count=0;
  while(top!=NULL && count!=5)
  {
    if(top->url!=NULL)
    {
      printf("%s\n", top->url);
      count++;
    }
    top=top->next;
  }
}
/*converts string to lowercase, used to lowercase users
 *search as tokens are stored in lowercase*/
void stringLwr(char* s)
{
  int i=0;
  while(s[i])
  {
    s[i]=tolower(s[i]);
    i++;
  }
}
/*intializes top struct and sets everything but pagerank equal to NULL
 *returns a top structure*/
struct top* createtop()
{
  struct top* t=(struct top*)malloc(sizeof(struct top));
  t->url=NULL;
  t->pr=1;
  t->next=NULL;
  return t;
}
/*sorts urls by pagerank from highest to lowest*/
struct top* sort(struct top* head,char* url,float pr)
{
  struct top* curr=head;
  struct top* new=createtop();
  new->pr=pr;
  new->url=url;
  if(curr->url==NULL || curr->pr < new->pr)
  {
    new->next=curr;
    return new;
  }
  else
  {
    while(curr->next!=NULL && curr->next->pr > new->pr)
    {
      curr=curr->next;
    }
    new->next=curr->next;
    curr->next=new;
    curr=head;
    return curr;
  }
}
/*assume hashelt is not empty,adds words in the HashElt list
 *that is what user specifies into a struct top which will then
 *order by pageranks*/
struct top* search(struct HashElt* head,char* key)
{
  struct HashElt* temp=head;
  struct top* list=createtop();
  stringLwr(key);
  while(temp!=NULL)
  {
    if(strcmp(key,temp->token)==0)
    {
      list=sort(list,temp->url,temp->pageRank);
    }
    temp=temp->next;
  }
  return list;
}
  

