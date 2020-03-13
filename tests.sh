#!/bin/sh

echo "*** data structure tests ***"
echo "*** pq"
./sp pq
echo "*** bfs"
./sp bfs
echo "*** bellman-ford"
./sp bellman-ford
echo "*** bellman-ford (negative weights)"
./sp bellman-ford-neg
echo "*** dijkstra"
./sp dijkstra
echo "*** astar"
./sp dijkstra astar

echo
echo "*** airport routing tests ****"

for algo in bfs bellman-ford dijkstra astar; do
    echo "*** algorithm: $algo ***"
   ./ap route $algo MAN MAN
   ./ap route $algo MAN SYD
   ./ap route $algo MAN MUC
   ./ap route $algo MAN ROA

   ./ap route $algo TTA MAN
   ./ap route $algo MAN TTA
   echo
   echo
done

echo
echo "*** airport reachability count tests ***"
./ap count MAN
./ap count SYD
./ap count TTA
