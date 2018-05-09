/****************************/
/* Hector Castillo Martinez */
/* CS 241                   */
/****************************/

#include "graph.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct queue
{
  struct node* curr;
  struct queue* next;
};

/*creates a graph with children now having edge to
 *to the parent*/
struct graph* reverseGraph(struct graph* g)
{
  struct graph* temp=createGraph();
  struct node* curr=g->firstNode;
  struct edge* edges;
  while(curr!=NULL)/*inserts all nodes into graph*/
  {
    insertNode(temp,curr->url);
    curr=curr->next;
  }
  curr=g->firstNode;
  while(curr!=NULL)/*loops until all nodes iterated*/
  {
    edges=curr->edges;
    while(edges!=NULL)/*loops until edges all inserted*/
    {
      insertEdge(temp,edges->connectsTo->url,curr->url);
      edges=edges->next;
    }
    curr=curr->next;
  }
  return temp;
}
/*Expects a graph and a url, url used to find the
 *node of which to count the number of edges of.
 *Used to calculate pagerank*/
int edgeCount(struct graph* g,char* url)
{
  int count=0;
  struct node* current=g->firstNode;
  struct edge* temp;
  while(current!=NULL)
  {
    if(strcmp(current->url,url)==0)/*enters when correct node found*/
    {
      temp=current->edges;
      while(temp!=NULL)
      {
        count++;
        temp=temp->next;
      }
      return count;
    }
    current=current->next;
  }
  return count;    
}
/*Calculates the pagerank of each node in the graph*/
void calculatePageRank(struct graph* g)
{
  int i;
  float pr=0;
  struct node* current;
  struct node* swap=g->firstNode;
  struct graph* temp=reverseGraph(g);
  struct edge* edges;
  for(i=0;i<100;i++)
  {
    current=temp->firstNode;
    while(current!=NULL)
    {
      edges=current->edges;
      /*goes through nodes of reverse graph and calculates pagerank*/
      while(edges!=NULL)
      {
        pr+=(edges->connectsTo->pagerank)/edgeCount(g,edges->connectsTo->url);
        edges=edges->next;
      }
      current->pagerank=(.15)+(.85*pr);/*sets node to calculated PR*/
      current=current->next;
      pr=0;/*resets to zero for next calc*/
    }
  }
  current=temp->firstNode;
  while(current!=NULL)/*sets pagerank calculated in reverse graph to nodes in actual graph*/
  {
    swap->pagerank=current->pagerank;
    current=current->next;
    swap=swap->next;
  }
}

/*prints out each URL and their associated pagerank*/
void printPageRank(struct graph* g,char* file)
{
  struct node* current=g->firstNode;
  FILE*fn=fopen(file,"w+");
  if(fn==NULL)
  {
    perror("This file");
    return;
  }
  
  while(current!=NULL)
  {
    fprintf(fn,"%s,%1.2f\n",current->url,current->pagerank);
    current=current->next;
  }
  fclose(fn); 
}

/*Goes through graph setting all visited flags of
 *nodes to unvisited*/
void  resetFlag(struct graph* g)
{
  struct node* current=g->firstNode;
  while(current!=NULL)
  {
    current->visited=0;
    current=current->next;
  }
}

/*Expects a node and a url as the arguments, traverses
 *through the node under it until it is NULL, then moves
 *to adjacent child. Returns 1 if the url is found else
 *returns 0*/
int depthHelper(struct node* a,char* url)
{
  struct edge* temp=a->edges;
  a->visited=1;
  if(strcmp(a->url,url)==0)return 1;/*url found in node*/
  while(temp!=NULL)
  {
    if(temp->connectsTo->visited==1);/*already visited*/
    else if(depthHelper(temp->connectsTo,url)==1)
    {
      return 1;
    }
    temp=temp->next;
  }
  return 0;
}
/*helper for readGraph, used to check if the 
 *node read in from file is already in the graph
 *returns 1 if found,else 0*/
int nodeExists(struct graph* g,char* url)
{
  struct node* curr=g->firstNode;
  while(curr!=NULL)
  {
    if(strcmp(curr->url,url)==0)return 1;
    curr=curr->next;
  }
  return 0;
}
/*reads in the graph from file and stores it in a
 *graph object. Expects a file name(string) to and
 *opens said file to read*/
struct graph* readGraph(char* fileName)
{
  struct graph* g=createGraph();
  char* url1=malloc(sizeof(char)*50);
  char* url2=malloc(sizeof(char)*50);
  char c;
  int index,fill=0;
  FILE* ptr=fopen(fileName,"r");/*opens file, reading purposes*/
  if(ptr==NULL)
  {
    perror("Error");
    return NULL;
  }  
  while((c=fgetc(ptr))!=EOF)
  {
    if(c!=',' && c!='\n')
    {
      if(fill==0)
      {
        url1[index]=c;
        index++;
      }
      else
      {
        url2[index]=c;
        index++;
      }
    }
    
    if(c==',')
    {
      fill++;
      index=0;
    }
    else if(c=='\n')
    {
      fill=0;
      index=0;
      /*checks if node already exists in graph*/
      if(nodeExists(g,url1)==0)insertNode(g,url1);
      if(nodeExists(g,url2)==0)insertNode(g,url2);
      insertEdge(g,url1,url2);
      url1=(char*)calloc(50,sizeof(char));/*resets strings for next line*/
      url2=(char*)calloc(50,sizeof(char));
    }
  }
  fclose(ptr);
  return g;
}
/*expects a node to initialze head, allocates
 *memory and sets next to null, returns 
 *initialized queue*/
struct queue* createQueueNode(struct node* n)
{
  struct queue* new=(struct queue*) malloc(sizeof(struct queue));
  new->curr=n;
  new->next=NULL;
  return new;
}
/*removes head from queue, making
 *item next to it the new head*/
struct node* dequeue(struct queue** headref)
{
  struct queue* temp=*headref;
  struct node* curr;
  if(temp==NULL)
  {
    curr=NULL;
    return curr;
  }
  curr=temp->curr;
  (*headref)=temp->next;
  free(temp);
  return curr;
}
/*Expects a queue and node as arguments. Node is to
 *be inserted in the queue and is put at the end
 *of the queue*/
struct queue* enqueue(struct queue* line,struct node* insert)
{
  struct queue* newNode=createQueueNode(insert);
  struct queue* current=line;
  if(current==NULL)
  {
    newNode->next=current;
    return newNode;
  }
  else
  {
    while(current->next!=NULL)
    {
      current=current->next;
    }
    newNode->next=current->next;
    current->next=newNode;
    return line;
  }
}
/* Uses breadth first search to find URL (specified in the
 *arguments) in the graph. If found returns 1, else returns
 *0. Uses queue to implement searching by adding all of 
 *the children of each node when dequeue is called on queue*/
int breadthFirstSearch(struct graph* g, char* url)
{
  struct node* current=g->firstNode;
  struct edge* temp=current->edges;
  struct queue* line=createQueueNode(current);
  resetFlag(g);
  if(current==NULL)return 0;/*empty graph*/
  else
  {
    while((current=dequeue(&line))!=NULL)
    {
      current->visited=1;
      temp=current->edges;/*gets first node to child*/
      if(strcmp(current->url,url)==0)/*checks for URL*/
      {
        resetFlag(g);
        return 1;
      }
      else
      {
        /*adds children if not already visited, stops when all added*/
        while(temp!=NULL)
         {
          if(temp->connectsTo->visited==1);
          else
          {
            line=enqueue(line,temp->connectsTo);
            temp->connectsTo->visited=1;
          }
          temp=temp->next;
        }
      } 
    }
  }
  resetFlag(g);
  return 0;
}
/*Uses depth first search to find the URL in the graph.
 *Returns 1 if url is in the graph*/
int depthFirstSearch(struct graph* g,char* url)
{
  struct node* current=g->firstNode;
  resetFlag(g);
  if(current==NULL)return 0;
  else if(depthHelper(current,url)==1)return 1;
  resetFlag(g);
  return 0;
}

/*int main(int argc,char* argv[])
{
  char* filein=argv[1];
  char* fileout=argv[2];
  struct graph* g;
  if(argc>3 || argc<3)
  {
    printf("Invalid amount of args\n");
    return 0;
  }
  g=readGraph(filein);
  calculatePageRank(g);
  printPageRank(g,fileout);
  return 0;
  }*/
