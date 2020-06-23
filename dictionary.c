// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = LENGTH * (int)'z';
static  int M = 0;

// Hash table
node *table[N];


void add_word(char *word, int hash)
{
  /* pointer to walk */ 
  node* ptr = table[hash]; 
  /* malloc new data */
  node *data = (struct node*) malloc(sizeof( node));
  data->next = NULL;

  /* fill in the data */
  strcpy(data->word, word);

  /* first element ?*/
  if (table[hash] == NULL)
  {
    table[hash] = data;
  }
  else
  {
    /* go the last element */
    while(ptr->next != NULL) 
    {
      ptr = ptr->next;
    }
    /* last elements next is not null, it's the new element !! */
    ptr->next = data;
  }

}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lowered[LENGTH+1];
    strcpy(lowered,word); 
    for(int i=0;lowered[i];i++)
    {
      lowered[i] = tolower(lowered[i]); 
    }
    node *ptr = table[hash(lowered)];
    if(ptr == NULL)
    {
      return false;
    }
    while(ptr != NULL)
    {
      if(!strcmp(ptr->word, word))
      {
        return true;
      }
      ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    int w_len = strlen(word);

    if(w_len == 0)
    {
      return 0;
    }

    for(int i=0;i<w_len;i++)
    {
      sum += word[i];
    }
    return sum;
}

int max_hash(int *a, int *b)
{
  return (*a > *b)?*a:*b;
}

// Loads dictionary into memory, returning true if successful else false
// to determine size we must find the max hash number 
// io is expensive, so store everything in a big string array also todo
bool load(const char *dictionary)
{
    int read = 0; 
    int len = 0;
    char line[LENGTH+1];
    int tmp_hash = 0;

    FILE *fp = fopen(dictionary, "r");

    if(fp == NULL)
    {

      printf("Can't open file %s\n",dictionary);

      return false;
    }

    while((fgets(line,LENGTH, fp)) != NULL)
    {
      tmp_hash = hash(line);
      line[strcspn(line,"\n")] = 0;
      M = max_hash(&tmp_hash, &M);
    }

    /* now we have the size */
    for(int i=0;i<M;i++)
    {
      table[i] = NULL;
    }

    /* read file again? */
    fseek(fp, 0, SEEK_SET);

    while(fgets(line,LENGTH, fp) != NULL)
    {
      line[strcspn(line,"\n")] = 0;
      tmp_hash = hash(line);
      /* check if already exists */
      add_word(line,tmp_hash);
    }


    return true;
}

unsigned int words_in(int hash_val)
{
  int sum = 0;

  if(hash_val > M)
  {
    return 0;
  }
  node *ptr = table[hash_val];
  if(ptr == NULL)
  {
    return 0;
  }
  while(ptr->next != NULL)
  {
    sum ++;
    ptr = ptr->next;
  }

  return sum;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned int sum = 0;

    /* traverse all of them and sum it up here */ 
    for(int i =0; i<M;i++)
    {
      sum += words_in(i);
    }
    
    return sum;
}

void node_free(int hash_val)
{
  node* tmp;
  while(table[hash_val] != NULL)
  {
    tmp = table[hash_val];
    table[hash_val] = table[hash_val]->next;
    free(tmp);
  }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0 ; i<M;i++)
    {
      if(table[i] != NULL)
      {
        node_free(i);
      }
    }
    return true;
}
