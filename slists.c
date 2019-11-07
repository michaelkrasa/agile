#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "slists.h"

struct person* insert_start(struct person* head, char *name, int age)
{
  struct person* new = (person*)malloc(sizeof(struct person));
  new->name = malloc(sizeof(char)*strlen(name)+1);
  strcpy(new->name, name);
  new->age = age;
  new->next = head;
  head = new;
  return head;
}

struct person* insert_end(struct person* head, char *name, int age)
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

struct person* insert_sorted(struct person* head, char *name, int age, int (*compare_people)(struct person*, struct person*))
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
