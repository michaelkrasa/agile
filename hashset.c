#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#define __USE_BSD
#include <string.h>
#include <math.h>

#include "global.h"
#include "hashset.h"

// Can be redefined if Value_Type changes
int compare(Value_Type a, Value_Type b){
  return strcmp(a,b);
}

// Helper functions for finding prime numbers
bool isPrime (int n)
{
  int i;
  for (i = 2; i*i <= n; i++)
    if (n % i == 0)
      return false;
  return true;
}

int nextPrime(int n)
{
  for (; !isPrime(n); n++);
  return n;
}

// Your code
struct hashset* initialize_set (int size)
{
  // TODO create initial hash table
  struct hashset* set = malloc(sizeof(struct hashset));
  set->cells = malloc(size * sizeof(cell));
  set->size = size;
  set->num_entries = 0;
  int i;
  for(i=0; i < size; i++)
    set->cells[i].state = 0;
  return set;
}

int generateHash(Value_Type value)
{
  int hash = 0;
  int i;
  for(i=0; i < strlen(value); i++)
  {
    if(i == 0)
      hash += 2*value[i];
    else
      hash += value[i];
  }
  return (int)(hash / strlen(value));
}

void tidy(struct hashset* set)
{
// TODO tidy up
}

int size(struct hashset* set){
  // TODO return number of values stored in table
  int i;
  int noOfCells = 0;
  for(i = 0; i < set->size; i++)
    if(set->cells[i].state != 0)
      noOfCells++;

  return noOfCells;
}

struct hashset* insert (Value_Type value, struct hashset* set)
{
  // TODO code for inserting into hash table
  int hash = generateHash(value);
  while(set->cells[hash].state != 0)
  {
    //if(hash = set->size - 1)
    hash++;
  }

  set->cells[hash].element = strdup(value);
  set->cells[hash].state = 1;
  return set;
}

struct hashset* resize(struct hashset* set)
{
  struct hashset* newSet = malloc(sizeof(struct hashset));
  newSet->cells = malloc(set->size * 2 * sizeof(cell));
  newSet->size = set->size * 2;
  newSet->num_entries = set->num_entries;

  int i;
  for(i=0; i < set->size; i++)
  {
    newSet->cells[i].element = strdup(set->cells[i].element);
    newSet->cells[i].state = set->cells[i].state;
  }
  free(set);
  return newSet;
}

bool find (Value_Type value, struct hashset* set)
{
  // TODO code for looking up in hash table
  int hash = generateHash(value);
  while(set->cells[hash].state != 0)
  {
    if(compare(set->cells[hash].element, value) == 0)
      return true;

    if(hash == set->size - 1)
      return false;
    hash++;
  }
  // Default
  return false;
}

void print_set (struct hashset* set)
{
  // TODO code for printing hash table
  int i;
  for(i = 0; i < set->size; i++)
  {
    if(set->cells[i].state == 1)
      printf("%s", set->cells[i].element);
  }
}

void print_stats (struct hashset* set)
{
  // TODO code for printing statistics

}
