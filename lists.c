#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arrays.h"

/* declare your struct for a person here */
struct person {
  char* name;
  int age;
  struct person* next;
};

static void insert(person* head, char *name, int age)
{
  Person* new = (Person*)malloc(sizeof(struct Person));
  new->name = (Person*)malloc(sizeof(char)*sizeof(names));
  new->name = name;
  new->age = age;

  new->next = head;
  head = new;
  /* modify nextfreeplace here */
  
}

int main(int argc, char **argv)
{
  int* testMalloc = malloc(15000000);
  if(testMalloc == NULL)
    return 1;

  free(testMalloc);

  // Declaring people array yeet
  person *head = NULL;
  nextinsert = 0;
  int i = 0;
  for (i=0;i < HOW_MANY; i++)
    insert(head, names[i], ages[i]);

  return 0;
}
