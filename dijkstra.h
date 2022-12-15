#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "List.h"

typedef struct Path
{
   int dist;
   IntList path;
} Path;

void printSolution(int* dist, const int vertCount);
Path* dijkstra(int** graph, int src, const int vertCount);

#endif //DIJKSTRA_H