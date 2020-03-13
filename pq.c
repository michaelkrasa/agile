#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#include "general.h"
#include "graph.h"
#include "pq.h"
#include "weight.h"

/**
 * Implement the interface given in pq.h.
 *
 * We make a few proposals here, but you are welcome to deviate from that, as long as you
 * correctly implement the interface with the specified worst-case complexities!
 *
 */

/**
 * First, we propose a 1-based indexing into the heap, using 0 as "invalid index".
 *
 * Wrapping a heap-index into a structure prevents its accidental confusion with element indexes (Which might save you from a lot of hard-to-find bugs!).
 *
 */

typedef struct {size_t i;} hidx_t;


const hidx_t INVALID_HIDX = {0};
const hidx_t hidx_first = {1};

size_t idx_of(hidx_t i) {return i.i - 1;} /// Convert to zero-based index
bool hidx_is_invalid(hidx_t i) {return i.i == 0;}

// Index operations
hidx_t parent(hidx_t i) {return (hidx_t){i.i / 2}; }
hidx_t left(hidx_t i) {return (hidx_t){i.i * 2}; }
hidx_t right(hidx_t i) {return (hidx_t){i.i * 2 + 1}; }
bool has_parent(hidx_t i) {return parent(i).i>0; }
bool has_left(size_t n, hidx_t i) {return left(i).i<=n; }
bool has_right(size_t n, hidx_t i) {return right(i).i<=n; }

/**
 * The priority queue has 3 main components:
 *   1. An array D that maps nodes to priorities. It is independent from the content of the actual queue.
 *   2. An array H of nodes that encodes the heap of size heap_size.
 *   3. An array I, that maps each node to its current index in H. If a node u is not in the queue, we set H[u] = INVALID_HIDX.
 *
 */
struct _DPQ_t {
    size_t N;     // Number of elements
    size_t heap_size;    // Current size of heap
    weight_t *D;  // Priorities
    node_t *H;    // The heap
    hidx_t *I;    // Index of element in heap.
};

DPQ_t *DPQ_new(size_t N) {
    // Initialize the structure. Allocate enough space for all arrays to hold N elements.
    DPQ_t *pq = malloc(sizeof(DPQ_t));
    pq->N = N;
    pq->heap_size = 0;
    pq->D = (weight_t *)malloc(sizeof(weight_t) * N);
    pq->H = (node_t *)malloc(sizeof(node_t) * N);
    pq->I = (hidx_t *)malloc(sizeof(hidx_t) * N);

    int i;
    for(i=0; i<(int)N; i++) {
      pq->D[i] = weight_inf();
	  pq->I[i] = INVALID_HIDX;
	  pq->H[i] = INVALID_NODE;
	}

    return pq;
}

void DPQ_delete(DPQ_t *pq) {
    // Free everything in pq
    free(pq->D);
    free(pq->H);
    free(pq->I);
    // and pq itself
    free(pq);
}

weight_t *DPQ_dist_free(DPQ_t *pq) {
  weight_t *res=pq->D;

  // Free everything but the D-array, which is returned
  free(pq->H);
  free(pq->I);
  free(pq);
  return res;
}

weight_t DPQ_prio(DPQ_t const *pq, node_t u) {
  // Its a good idea to use assertions to ensure the preconditions!
  // This makes programs more likely to fail close to the reason of the error, rarher than later, when the (indirect) consequences become apparent.
  assert(u<pq->N);

  return pq->D[u]; /// *** return priority of node u
}

/**
 * It is a good idea to wrap repetitive patterns into an own function (though, in C, it can be a pain).
 * This reduces the redundancy, and the chance of accidentally getting one of the repeated versions wrong.
 *
 * In this case, we get the priority associated with a heap index.
 */
weight_t _DPQ_hprio(DPQ_t const *pq, hidx_t i) {
    return DPQ_prio(pq,pq->H[idx_of(i)]);
}

void _DPQ_swap(DPQ_t *pq, hidx_t i, hidx_t j) {

  // Heap and index swap
  node_t tempNode = pq->H[idx_of(i)];

  pq->I[pq->H[idx_of(i)]] = j;
  pq->H[idx_of(i)] = pq->H[idx_of(j)];

  pq->I[pq->H[idx_of(j)]] = i;
  pq->H[idx_of(j)] = tempNode;
}

void _DPQ_sift_up(DPQ_t *pq, hidx_t i) {
  
  while(has_parent(i)) {
	  hidx_t p = parent(i);
    if(weight_less(_DPQ_hprio(pq, i), _DPQ_hprio(pq, p)))
      _DPQ_swap(pq, p, i);
      
    else
      break;

    i = p;
 } // while
}

void _DPQ_sift_down(DPQ_t *pq, hidx_t i) {

  // Don't forget to handle the case that you end up at an index that has a left, but no right child!
     
    while(has_left(pq->heap_size, i)) {
      if(!has_right(pq->heap_size, i)) {
        if(weight_less(_DPQ_hprio(pq, left(i)), _DPQ_hprio(pq, i)))
          _DPQ_swap(pq, i, left(i));
        break;
      }
      
      hidx_t smallest = left(i);
      
      if(weight_less(_DPQ_hprio(pq, right(i)), _DPQ_hprio(pq, left(i))))
        smallest = right(i);
        
      if(weight_less(_DPQ_hprio(pq, smallest), _DPQ_hprio(pq, i))) {
        _DPQ_swap(pq, i, smallest);
        i = smallest;
      } else
          break;
    }     
  }

bool DPQ_contains(DPQ_t const *pq, node_t u) {

    assert(u<pq->N);
    return pq->I[u].i != INVALID_HIDX.i; // Use I to check if node is on heap
}

void DPQ_insert(DPQ_t *pq, node_t u, weight_t w) {
  // Insert at end, then sift up
  // Don't forget to update I!
  assert(pq->heap_size < pq->N);
  assert(!DPQ_contains(pq,u));
  assert(u < pq->N);

  pq->H[pq->heap_size] = u;
  pq->D[u] = w;
  pq->heap_size++;
  pq->I[u].i = pq->heap_size;
  
  _DPQ_sift_up(pq, pq->I[u]);
}

bool DPQ_is_empty(DPQ_t const *pq) {return pq->heap_size==0;}

node_t DPQ_pop_min(DPQ_t *pq) {

  assert(pq->heap_size>0);
  node_t min = pq->H[0];  
  
  pq->heap_size--;
  _DPQ_swap(pq, hidx_first, pq->I[pq->H[pq->heap_size]]);
  pq->I[min] = INVALID_HIDX;

  _DPQ_sift_down(pq, hidx_first);

  return min; // Return the node that was removed
}

void DPQ_decrease_key(DPQ_t *pq, node_t u, weight_t w) {
  assert(u<pq->N);
  assert(DPQ_contains(pq,u));
  assert(weight_less(w, pq->D[u]));

  // Update priority, then sift up. Use I to find element's index on the heap
  pq->D[u] = w;
  _DPQ_sift_up(pq, pq->I[u]);
}
