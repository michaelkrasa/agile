#include "slists.h"
#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
