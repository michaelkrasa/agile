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

// same
struct hashset* initialize_set (int size)
{
  // Create initial hash table
  struct hashset* set = malloc(sizeof(struct hashset));
  set->cells = malloc(size * sizeof(cell));
  set->size = size;
  set->num_entries = 0;
  set->totCol = 0;
  set->collisionsPerInsertion = 0;

  int i;
  for(i=0; i < size; i++)
  {
    set->cells[i].element = NULL;
    set->cells[i].state = 0;
  }
  return set;
}

// Doubles our hashset size
struct hashset* resize(struct hashset* set)
{
  struct hashset* newSet = initialize_set(set->size * 2);

  newSet->num_entries = set->num_entries;
  newSet->totCol = set->totCol;
  newSet->collisionsPerInsertion = set->collisionsPerInsertion;

  int i;
  for(i=0; i < set->size; i++)
  {
    newSet->cells[i].element = set->cells[i].element;
    newSet->cells[i].state = set->cells[i].state;
  }

  printf("Size now: %d\n", newSet->size);
  tidy(set);
  return newSet;
}

// Generates a hash for the value in the table
int generateHash(Value_Type value, struct hashset* set)
{
  int hash = 0;
  int i;

  if(mode == 1)
  {
    // Add up all ASCII values
    for(i=0; i < strlen(value); i++)
      hash += value[i];

    // returns the next prime
    return hash / set->size;
  }

  // Mode 0 - linear probing
  else
  {
    for(i=0; i < strlen(value); i++)
      hash += value[i];

    hash = hash / strlen(value);
    return hash % set->size;
  }
  return -1;
}

// Frees all the memory
void tidy(struct hashset* set)
{
  // Tidy up all the cells
  int i;
  for(i = 0; i < set->size; i++)
    free(set->cells[i].element);

  free(set->cells);
  free(set);
}

int size(struct hashset* set){
  // Return number of values stored in table
  int i;
  int noOfCells = 0;
  for(i = 0; i < set->size; i++)
    if(set->cells[i].state != 0)
      noOfCells++;

  return noOfCells;
}

struct hashset* insert(Value_Type value, struct hashset* set)
{
  // Code for inserting into hash table
  int hash = generateHash(value, set);
  printf("Hash: %d\n", hash);
  int firstHash = hash;

  while(hash > set->size)
    set = resize(set);

  while(set->cells[hash].state != 0)
    hash++;

  // How many collisions before insert
  set->totCol += hash - firstHash;
  set->num_entries++;
  set->collisionsPerInsertion = (float) set->totCol / set->num_entries;
  set->cells[hash].element = strdup(value);
  set->cells[hash].state = 1;

  return set;
}

bool find (Value_Type value, struct hashset* set)
{
  // Code for looking up in hash table
  int hash = generateHash(value, set);

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
  // Code for printing hash table
  int i;
  for(i = 0; i < set->size; i++)
  {
    if(set->cells[i].state == 1)
      printf("Element: %s\n", set->cells[i].element);
  }
}

void print_stats (struct hashset* set)
{
  printf("Collisions per insertion: %f\n", set->collisionsPerInsertion);
}
