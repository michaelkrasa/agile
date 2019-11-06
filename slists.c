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

static int compare_people_by_name(struct person* p1, struct person* p2)
{
  return strcmp(p1->name, p2->name);
}

static int compare_people_by_age(struct person* p1, struct person* p2)
{
  if(p1->age < p2->age) {return -1;}
  else if(p1->age > p2->age) {return 1;}
  else {return 0;}
}



static person* insert_start(struct person* head, char *name, int age)
{
  struct person* new = (person*)malloc(sizeof(struct person));
  new->name = malloc(sizeof(char)*strlen(name)+1);
  strcpy(new->name, name);
  new->age = age;
  new->next = head;
  head = new;
  return head;
}

static person* insert_end(struct person* head, char *name, int age)
{
  struct person* new = (person*)malloc(sizeof(struct person));
  struct person* prev = head;

  new->name = malloc(sizeof(char)*strlen(name)+1);
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

static person* insert_sorted(struct person* head, char *name, int age, int (*compare_people)(struct person*, struct person*))
{
  struct person* new = (person*)malloc(sizeof(struct person));

  new->name = malloc(sizeof(char)*strlen(name)+1);
  strcpy(new->name, name);
  new->age = age;

  if (head == NULL || compare_people(new, head) < 0)
  {
    new->next = head;
    head = new;
    return head;
  }
  else
  {
    struct person* start = head;
    while(head->next != NULL && compare_people(new, head->next) >= 0)
      head = head->next;

    new->next = head->next;
    head->next = new;
    return start;
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
    else if(argc == 2 && strcmp(argv[1], "insert_sorted") == 0)
      head = insert_sorted(head, names[i], ages[i], compare_people_by_name);
    else if(argc == 3 && strcmp(argv[1], "insert_sorted") == 0 && strcmp(argv[2], "name") == 0)
      head = insert_sorted(head, names[i], ages[i], compare_people_by_name);
    else if(argc == 3 && strcmp(argv[1], "insert_sorted") == 0 && strcmp(argv[2], "age") == 0)
      head = insert_sorted(head, names[i], ages[i], compare_people_by_age);
    else
    {
      fprintf(stderr, "Nope\n");
      return 1;
    }
  }
  // Preserve the head before we iterate through it and print it
  struct person* start = head;

  // Printing
  while (head != NULL)
  {
    printf("Name: %s, age: %d\n", head->name, head->age);
    head = head->next;
  }
  // Resetting the head to be used in the next block
  head = start;

  // Freeing the memory
  while(start != NULL)
  {
    head = start->next;
    free(start->name);
    free(start);
    start = head;
  }

  return 0;
}
