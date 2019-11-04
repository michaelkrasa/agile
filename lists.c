#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data.h"

/* declare union for a person here */
typedef struct person {
  char* name;
  int age;
  struct person* next;
} person;

static person* insert_start(person* head, char *name, int age)
{
  struct person* new = (struct person*)malloc(sizeof(struct person));
  new->name = malloc(sizeof(char)*strlen(name));
  strcpy(new->name, name);
  new->age = age;
  new->next = head;
  head = new;
  return head;
}


static person* insert_end(person* head, char *name, int age)
{
  struct person* new = (struct person*)malloc(sizeof(struct person));
  struct person* prev = head;

  new->name = malloc(sizeof(char)*strlen(name));
  strcpy(new->name, name);
  new->age = age;

  if(head == NULL)
  {
    new->next = head;
    head = new;
    return head;
  }

  while (prev->next != NULL)
    prev = prev->next;

  prev->next = new;
  return head;
}

/*static person* insert_sorted(struct person* head, char *name, int age)
{
  struct person* new = (struct person*)malloc(sizeof(struct person));
  struct person* prev = head;

  new->name = malloc(sizeof(char)*strlen(name));
  new->name = name;
  new->age = age;

  if (head == NULL)
  {
     head = new;
     return head;
  }

  while (prev->next != NULL)
    prev = prev->next;

  prev->next = new;
  return head;
}*/

void printList(struct person *node)
{
  while (node != NULL)
  {
    printf("Name: %s, age: %d\n", node->name, node->age);
    node = node->next;
  }
}

int main(int argc, char** argv)
{
  int* testMalloc = malloc(15000000);
  if(testMalloc == NULL)
    return 1;

  free(testMalloc);

  // Starting with an empty list
  struct person* head = NULL;

  int i = 0;
  for (i=0;i < HOW_MANY; i++)
  {
    if(argc == 1 || argc == 2 && strcmp(argv[1], "insert_start") == 0)
      head = insert_start(head, names[i], ages[i]);
    else if(argc == 2 && strcmp(argv[1], "insert_end") == 0)
      head = insert_end(head, names[i], ages[i]);
    else
    {
      fprintf(stderr, "U fucked up m8\n");
      return 1;
    }
  }
  // Preserve the head before we iterate through it and print it
  struct person* start = head;

  printList(head);

  /*for(i=0; i < HOW_MANY; i++)
  {
    free(people[i]->name);
    free(people[i]);
  }*/

  return 0;
}
