// bnb.c
// (C) Joshua Knowles
// j.knowles@manchester.ac.uk

//branch and bound

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>  // we are using assert.h to check some conditions remain true
#include <math.h>
#include <stdbool.h>
#define DOUB_MAX 10e30 // a large number, must be greater than max value of any solution
# define SIZE 100000 // an estimate of how large the priority queue could become
# define NITEMS 2000 // an upper limit of the number of items

FILE * fp; // file pointer for reading the input files
int Capacity; // capacity of the knapsack (total weight that can be stored)
int Nitems; // number of items in the knapsack problem instance
int * item_weights; // vector of item weights
int * item_values; // vector of item profits or values
int * temp_indexes; // temporary item indexes for sorting items by value/weight
int QueueSize = 0; // the number of items currently stored in the priority queue
int QUIET = 0; // can be set to 1 to suppress output

// the structure for a solution. All of this will be stored in the priority queue
typedef struct solution_structure {
  int solution_vec[NITEMS + 1]; // binary solution vector;
  //e.g. solution_vec[1]=1 means first item is packed in knapsack.
  //     solution_vec[1]=0 means first item is NOT in knapsack.
  // NB: solution_vec[0] is meaningless.

  int val; // its value
  int fixed; // the number of items that are fixed to either 0 or 1, not '*'
  double bound; // the upper bound value of the solution
}
struc_sol;

struc_sol * pqueue; // the knapsack solutions. To be stored in a priority queue

// function prototypes
extern void read_knapsack_instance(char * filename);
extern void sort_by_ratio();
extern int check_evaluate_and_print_sol(int * sol, int * total_value, int * total_weight);
void branch_and_bound(int * final_sol);
void copy_array(int * from, int * to);

// The following four functions implement a priority queue.
// They are based on the functions given in Robert Sedgwick's book, Algorithms in C.
void upheap(int qsize) {
  // upheap reorders the elements in the heap (queue) after an insertion

  struc_sol temp_element;
  temp_element = pqueue[qsize];
  pqueue[0].bound = DOUB_MAX;

  while (pqueue[qsize / 2].bound <= temp_element.bound) {
    pqueue[qsize] = pqueue[qsize / 2];
    qsize = qsize / 2;
  }
  pqueue[qsize] = temp_element;
}

void insert(struc_sol element) {
  assert(QueueSize < SIZE - 1);
  pqueue[++QueueSize] = element;
  upheap(QueueSize);
}

void downheap(int qindex) {
  // down heap reorders the elements in the heap (queue) after a removal

  int j;
  struc_sol temp_element;
  temp_element = pqueue[qindex];
  while (qindex <= QueueSize / 2) {
    j = qindex + qindex;
    if (j < QueueSize && pqueue[j].bound < pqueue[j + 1].bound)
      j++;
    if (temp_element.bound >= pqueue[j].bound)
      break;
    pqueue[qindex] = pqueue[j];
    qindex = j;
  }
  pqueue[qindex] = temp_element;
}

struc_sol removeMax() {
  struc_sol head = pqueue[1];
  pqueue[1] = pqueue[QueueSize--];
  downheap(1);
  return head;
}
// End priority queue functions.

void print_sol(struc_sol * sol) {
  // prints a solution in the form 000100xxxx etc
  // with x's denoting the part of the solution not yet fixed (determined)

  int i;
  printf("%d %g ", sol->val, sol->bound);
  for (i = 1; i <= sol->fixed; i++)
    printf("%d", sol->solution_vec[i]);
  while (i <= Nitems) {
    printf("x");
    i++;
  }
  printf("\n");
}

void frac_bound(struc_sol * sol, int fix) {
  // Updates the values sol->val and sol->bound

  // Computes the fractional knapsack upper bound
  // given a binary vector of items (sol->solution_vec),
  // where the first
  // "fix" of them are fixed. All that must be done
  // is compute the value of the fixed part; then
  // add to that the value obtained by adding in
  // items beyond the fixed part until the capacity
  // is exceeded. For the exceeded capacity, the fraction
  // of the last item added which would just fill the knapsack
  // is taken. This fraction of profit/value is added to the
  // total. This is the required upper bound.

  // Everything above assumes items are sorted in decreasing
  // profit/weight ratio

  int i; // index of current item
  double totalp = 0; // profit total
  int totalw = 0; // weight total
  sol->val = -1;

  // compute the current value and weight of the fixed part
  for (i = 1; i <= fix; i++) {
    if (sol->solution_vec[i] == 1) {
      totalw += item_weights[temp_indexes[i]];
      totalp += item_values[temp_indexes[i]];
    }
  }
  if (totalw > Capacity) // if fixed part infeasible, return
    return;

  sol->val = totalp;
  //  printf("%g %d\n", totalp, totalw);

  // add in items the rest of the items until capacity is exceeded
  i = fix + 1;
  do {
    // update totalw and totalp
    totalw += item_weights[temp_indexes[i]];
    totalp += item_values[temp_indexes[i]];
    i++;
  } while ((i <= Nitems) && (totalw < Capacity));

  /* if over-run the capacity, adjust profit total by subtracting
     that overrun fraction of the last item  */
  if (totalw > Capacity) {
    --i;
    totalp -= ((double)(totalw - Capacity) / (double)(item_weights[temp_indexes[i]])) * item_values[temp_indexes[i]];
  }
  sol->bound = totalp;
}

int main(int argc, char * argv[1]) {
  int * final_sol; // binary solution vector indicating items to pack
  int total_value, total_weight; // total value and total weight of items packed

  read_knapsack_instance(argv[1]);

  assert(NITEMS >= Nitems);

  if ((final_sol = (int * ) malloc((Nitems + 1) * sizeof(int))) == NULL) {
    fprintf(stderr, "Problem allocating solution vector final_sol\n");
    exit(1);
  }

  sort_by_ratio();

  if ((pqueue = (struc_sol * ) malloc(sizeof(struc_sol) * SIZE)) == NULL) {
    fprintf(stderr, "Problem allocating memory for priority queue. Reduce SIZE.\n");
    exit(1);
  }

  branch_and_bound(final_sol);
  printf("Branch and Bound Solution of Knapsack is:\n");
  check_evaluate_and_print_sol(final_sol, & total_value, & total_weight);
  return (0);
}

// Creates child, adds number of fixed items and copies solution vector
struc_sol* makeChild(struc_sol parent) {
  struc_sol* child = malloc(sizeof(struc_sol));
  child->fixed = parent.fixed + 1;
  copy_array(parent.solution_vec, child->solution_vec);
  return child;
}

void branch_and_bound(int * final_sol) {
  // branch and bound

  // start with the empty solution vector
  // compute its value and its bound
  // put current_best = to its value
  // store it in the priority queue

  // LOOP until queue is empty or upper bound is not greater than current_best:
  //   remove the first item in the queue
  //   construct two children, 1 with a 1 added, 1 with a O added
  //   FOREACH CHILD:
  //     if infeasible, discard child
  //     else
  //       compute the value and bound
  //       if value > current_best, set current_best to it, and copy child to final_sol
  //       add child to the queue
  // RETURN

  // Empty struct to be added to the pqueue
  struc_sol sol;
  frac_bound(&sol, 0); // calculated bound and value
  int current_best = sol.val;
  insert(sol);

  struc_sol* child0;
  struc_sol* child1;

  // Main body of the method
  while(QueueSize >= 1 && sol.bound > current_best) {

    struc_sol max = removeMax(); // pop pq

    // Create children using function
    child0 = makeChild(max);
    child1 = makeChild(max);

    // Adding 1 and 0 to the solution vector
    child0->solution_vec[child0->fixed] = 0;
    child1->solution_vec[child1->fixed] = 1;

    // Calculate total values and bounds of the children
    frac_bound(child0, child0->fixed); // val will be -1 if infeasible
    frac_bound(child1, child1->fixed);

    // If child0 is feasible - determined by frac_bound and isn't too long
    if (child0->val == -1 || child0->fixed >= Nitems) {
      free(child0);
    }
    else { // if feasible and higher value
      if(child0->val > current_best) {
        current_best = child0->val;
        copy_array(child0->solution_vec, final_sol); // copy solution
        sol.bound = child0->bound;
      }
      insert(*child0);
    }

    // If child1 is feasible
    if (child1->val == -1 || child1->fixed >= Nitems) {
      free(child1);
    }
    else { // if feasible and higher value
      if(child1->val > current_best) {
        current_best = child1->val;
        copy_array(child1->solution_vec, final_sol); // copy solution
        sol.bound = child1->bound;
      }
      insert(*child1);
    }
  } // while
}

void copy_array(int * from, int * to) {
  // This copies Nitems elements of one integer array to another.
  // Notice it should be called with "to" and "from" pointing to the first, not the
  // zeroth, element of the solution_vec array

  int i;
  for (i = 0; i < Nitems; i++) {
    to[i] = from[i];
  }
}
