#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arrays.h"

/* declare your struct for a person here */
struct person {
  char* name;
  int age;
} person;

static void insert(struct person* people[], char *name, int age, int *nextinsert)
{
  /* put name and age into the next free place in the array parameter here */
  people[(*nextinsert)] = malloc(sizeof(person));
  people[(*nextinsert)]->name = malloc(sizeof(char)*sizeof(name));
  strcpy(people[(*nextinsert)]->name, name);
  people[(*nextinsert)]->age = age;
  /* modify nextfreeplace here */
  (*nextinsert)++;
}

int main(int argc, char **argv)
{
  int* testMalloc = malloc(15000000);
  if(!testMalloc)
    return 1;

  free(testMalloc);

  // Declaring people array yeet
  struct person* people[HOW_MANY];
  int nextinsert = 0;
  int i = 0;
  for (i=0;i < HOW_MANY; i++)
    insert(people, names[i], ages[i], &nextinsert);

  /* print the people array here*/
  for(i=0; i < HOW_MANY; i++)
  {
    printf("%d: %s is %d\n", i, people[i]->name, people[i]->age);
  }
  for(i=0; i < HOW_MANY; i++)
  {
    free(people[i]->name);
    free(people[i]);
  }

  return 0;
}
