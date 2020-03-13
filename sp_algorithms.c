#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>

#include "graph.h"
#include "pq.h"
#include "sp_algorithms.h"

void enQueue(node_t *q, size_t *rear,node_t node,size_t q_size){
  q[*(rear)] = node;
  (*rear)++;
  if(*rear >= q_size)
    *rear = 0;
}
node_t deQueue(node_t *q, size_t *front,size_t q_size){
  node_t popped = q[*(front)];
  (*front)++;
  if(*front >= q_size)
    *front = 0;
  return popped;
}

sssp_result_t *bfs(graph_t const *g, node_t src, node_t dst) {
  // Calloc and prep all variables
  unsigned long long stat_edges_explored=0;

  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  DPQ_t *dpq = DPQ_new(N);
  bool visited[N];

  // Initialize all nodes to inf weight
  for(int i = 0; i < (int)N; i++){
    visited[i] = 0;
    dist[i] = weight_inf();
  }

  visited[src] = 1;
  dist[src] = weight_zero();

  // Insert into our 'visited' dpq
  DPQ_insert(dpq, src, weight_zero());
  while(!DPQ_is_empty(dpq)){
    node_t curr = DPQ_pop_min(dpq);

    // Check if the nodes arent the same
    if(curr == dist)
      break;

    // Foe each adjecent node, set it to visited, insert it into dpq
    for(edge_tgt_t const *e = graph_succs_begin(g, curr); e != graph_succs_end(g, curr); ++e){
      if(visited[e->v] != true){
          visited[e->v] = true;
          pred[e->v] = curr;
          dist[e->v] = weight_add(dist[curr], weight_of_dbl(1));
          DPQ_insert(dpq, e->v, e ->w);
      }
    }
  }
  return sssp_result_new(N,src,dst,false,pred,dist,stat_edges_explored);
}

sssp_result_t *bellman_ford(graph_t const *g, node_t src) {

  unsigned long long seen = 0;
  bool neg_cycle = false;
  node_t n = INVALID_NODE;

  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  // Initialize
  for(size_t i = 0; i < N; i++){
    dist[i] = weight_inf();
    pred[i] = INVALID_NODE;
  }

  dist[src] = weight_zero();
  pred[src] = src;

  node_t new;
  bool relaxed;

  // Relax edges repeatedly
  for(size_t i = 0; i < N; i++){
    relaxed = false;
    for(size_t j = 0; j < N; j++){
      new = j;
      for(edge_tgt_t const *e = graph_succs_begin(g, new); e != graph_succs_end(g, new); ++e){
        node_t v = e->v;
        weight_t w = e->w;
        if(weight_less(weight_add(dist[new], w), dist[v])){
            dist[v] = weight_add(dist[new], w);
            relaxed = true;
            pred[v] = new;
        }
      }
    }
    if(!relaxed){
      break;
    }
  }
  // Check for negative weight cycles
  for(size_t i = 0; i < N; i++){
    new = i;
    for(edge_tgt_t const *e = graph_succs_begin(g, new); e != graph_succs_end(g, new); ++e){
      node_t v = e->v;
      weight_t w = e->w;
      // If it finds a negative weight path
      if(weight_less(weight_add(dist[new], w), dist[v])){
        dist[v] = weight_neg_inf();
        neg_cycle = true;
      }
     }
   }
  return sssp_result_new(N, src, n,neg_cycle, pred, dist, seen);
}

sssp_result_t *dijkstra(graph_t const *g, node_t src, node_t dst) {

  // Initialize
  unsigned long long stat_edges_explored=0;

  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  DPQ_t *dpq = DPQ_new(N);

  for(int i = 0; i < N; i++){
    dist[i] = weight_inf();
    pred[i] = INVALID_NODE;
    DPQ_insert(dpq, i, dist[i]);
  }

  if(negativeWeights(g, src)){
    DPQ_delete(dpq);
    return sssp_result_new(N, src, INVALID_NODE, false, pred, dist, stat_edges_explored);
  }
  pred[src] = src;
  dist[src] = weight_zero();

  DPQ_decrease_key(dpq, src, dist[src]);

  while(!DPQ_is_empty(dpq)){
    node_t node = DPQ_pop_min(dpq);

    if (node == dst)
      break;

    bool successful = false;

    for (edge_tgt_t const *e = graph_succs_begin(g, u);  e != graph_succs_end(g, u);  e++){
      stat_edges_explored++;
      // Relax the nodes

      if(successful)
        DPQ_decrease_key(dpq, e->v, dist[e->v]);

      successful = false;
    }
  }

  DPQ_delete(dpq);
  return sssp_result_new(N, src, INVALID_NODE, false, pred, dist, stat_edges_explored);
}

sp_result_t *astar_search(graph_t const *g, node_t src, node_t dst, weight_t const* h) {
  unsigned long long stat_edges_explored=0;

  size_t N = graph_get_num_nodes(g);
  node_t *pred = CALLOC(node_t,N);
  weight_t *dist = CALLOC(weight_t,N);

  DPQ_t *dpq = DPQ_new(N);

  int i;
  for(i = 0; i < N; i++){
    dist[i] = weight_inf();
    pred[i] = INVALID_NODE;
    DPQ_insert(dpq, i, dist[i]);
  }

  if(negativeWeights(g, src)){
    DPQ_delete(dpq);
    return sssp_result_new(N, src, INVALID_NODE, false, pred, dist, stat_edges_explored);
  }

  pred[src] = src;
  dist[src] = weight_zero();

  DPQ_decrease_key(dpq, src, dist[src]);

  while(!DPQ_is_empty(dpq)){
    node_t node = DPQ_pop_min(dpq);

    if (node == dst){
      break;
    }

    bool successful = false;

    for (edge_tgt_t const *e = graph_succs_begin(g, u);  e != graph_succs_end(g, u);  e++){
      stat_edges_explored++;

      // relax the nodes I don't know what to do with it

      if(successful){
        DPQ_decrease_key(dpq, e->v, weight_add(dist[e->v]), h[e->v]);
      }
      successful = false;
    }
  }

  DPQ_delete(dpq);

  return sssp_result_new((N, src, INVALID_NODE, false, pred, dist, stat_edges_explored), dst);
}
