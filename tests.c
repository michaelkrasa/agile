// Giles Reger, 2019

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "pq.h"

// Some helper functions for checking the results of calls in tests
void check_result(int testno, char* expected, char* found){
 if(found == NULL){
   fprintf(stderr,"Test %d Failed when %s expected and NULL found\n",testno,expected);
   exit(-1);
 }
 if(strcmp(expected,found) != 0){
   fprintf(stderr,"Test %d Failed when %s expected and %s found\n",testno,expected,found);
   exit(-1);
 }
}
void assert(int testno, bool value, char* reason){
  if(!value){
   fprintf(stderr,"Test %d Failed as %s\n",testno,reason);
   exit(-1);
  }
}

// Check that hello and goodbye are poped in the right order
void run_test0(){
  printf("TEST 0\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"hello",1);
  printf("Inserted hello with priority 1...\n");
  insert(queue,"goodbye",2);
  printf("Inserted goodbye with priority 2...\n");
  check_result(0,"hello",pop_min(queue));
  printf("Popped hello...\n");
  check_result(0,"goodbye",pop_min(queue));
  printf("Popped goodbye...\n");
  assert(0,is_empty(queue), "queue is meant to be empty");
  printf("Queue now empty\n");
  tidy(queue);
}

// Check that "contains" function works with keys
void run_test1(){
  printf("TEST 1\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"foo", 1337);
  printf("Inserted foo with priority 1337...\n");
  insert(queue,"bar", 400);
  printf("Inserted goodbye with priority 400...\n");
  assert(1, contains(queue, "foo", 1337), "foo is not in queue");
  printf("Contains foo...\n");
  assert(1, contains(queue, "bar", 400), "bar is not in queue");
  printf("Contains bar...\n");
  check_result(1,"bar",pop_min(queue));
  printf("Popped bar...\n");
  check_result(1,"foo",pop_min(queue));
  printf("Popped foo...\n");
  assert(1, is_empty(queue), "queue is not empty");
  tidy(queue);
}

// Check that priority queue can handle elements with the same key
void run_test2(){
  printf("TEST 2\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"first", 1);
  printf("Inserted first with priority 1...\n");
  insert(queue,"second", 1);
  printf("Inserted second with priority 1...\n");
  check_result(2, "second", pop_min(queue));
  printf("Popping first...\n");
  check_result(2, "first", pop_min(queue));
  printf("Popping second...\n");
  assert(2,is_empty(queue)," queue is empty");
  tidy(queue);
}

// Check that priority queue is empty when created
void run_test3(){
  printf("TEST 3\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  assert(3,is_empty(queue),"queue is not empty");
  printf("Queue is indeed empty\n");
  tidy(queue);
}

// Check that priority queue truly popped the first element
void run_test4(){
  printf("TEST 4\n");
  PriorityQueue queue = initialize_pq(10);
  printf("Initialised...\n");
  insert(queue,"first",1);
  printf("Inserted first with priority 1...\n");
  insert(queue,"second",2);
  printf("Inserted second with priority 1...\n");
  check_result(4,"first", pop_min(queue));
  printf("Popping first...\n");
  assert(4, contains(queue, "second", 1), "second is not in queue");
  printf("First isn't in the queue anymore\n");
  tidy(queue);
}

char* prog_name = 0;
int main (int argc, char *argv[])
{
 prog_name=argv[0];
 if(argc != 2){
   fprintf(stderr,"Supply test number\n");
   return -1;
 }
 char* tmp;
 int test_number = strtol(argv[1],&tmp,0);
 if(*tmp != '\0'){
   fprintf(stderr,"Supply test number as an integer\n");
   return -1;
 }
 switch(test_number){
   case 0: run_test0(); break;
   case 1: run_test1(); break;
   case 2: run_test2(); break;
   case 3: run_test3(); break;
   case 4: run_test4(); break;

   default:
     fprintf(stderr,"Test number %d not recognised\n", test_number);
 }
 return 0;
}
