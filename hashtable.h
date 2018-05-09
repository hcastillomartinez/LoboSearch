#ifndef HASHTABLE_H
#define HASHTABLE_H

struct HashElt
{
  char* url;
  float pageRank;
  char* token;
  struct HashElt* next;
};
struct HashElt* createHashElt();
struct HTable* createHtable();
struct HTable
{
  int size; /* size of the array*/
  int n; /* number of elements stored in the hash table*/
  struct HashElt* table[2000]; /* the array of pointers we store things in*/
};
/* Turns the token from the HashElt into a number by adding the ascii values of each char in the token string*/
int hash(struct HashElt* elt);


/* Hash the element and put it in the hashtable. If the element is already in the hashtable, do nothing.*/
void insert(struct HashElt* elt, struct HTable* table);

/* returns a linked list of everything that's token hashes to the same value as key */
struct HashElt* lookup(char* key, struct HTable* table);

#endif
