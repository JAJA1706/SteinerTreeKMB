#include "dijkstra.h"
#include "List.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
 
int minDistance(Path* path, bool* sptSet, const int vertCount)
{
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < vertCount; v++)
        if (sptSet[v] == false && path[v].dist <= min)
            min = path[v].dist, min_index = v;
 
    return min_index;
}

//reconstructs path based on previous vertices
int reconstructPath(Path* path, int* previousVerts, int src, const int vertCount)
{
   for(int i = 0; i < vertCount; ++i)
   {
      push_back(&path[i].path, i);
      if(i == src)
      {
         continue;
      }

      int prevVert = previousVerts[i];
      push_back(&path[i].path, prevVert);
      while( prevVert != src)
      {
         prevVert = previousVerts[prevVert];
         push_back(&path[i].path, prevVert);
      }
   }
}

void printSolution(int* dist, const int vertCount)
{
    printf("Vertex \t\t Distance from Source\n");
    for (int i = 0; i < vertCount; i++)
        printf("%d \t\t\t\t %d\n", i+1, dist[i]);
}

//returns dynamically shortest distance and path to all vectors from src
Path* dijkstra(int** graph, int src, const int vertCount)
{
   Path* shortestPath = malloc(sizeof(Path)*vertCount);
   int* previousVert = malloc(sizeof(Path)*vertCount);
   bool* sptSet = malloc(sizeof(bool)*vertCount);

   for (int i = 0; i < vertCount; i++)
   {
      shortestPath[i].dist = INT_MAX, sptSet[i] = false;
      shortestPath[i].path.head = NULL;
   }

   shortestPath[src].dist = 0;
   for (int count = 0; count < vertCount - 1; count++) {
      int u = minDistance(shortestPath, sptSet, vertCount);
      sptSet[u] = true;

      for (int v = 0; v < vertCount; v++)
         if (!sptSet[v] && graph[u][v]
         && shortestPath[u].dist != INT_MAX
         && shortestPath[u].dist + graph[u][v] < shortestPath[v].dist)
         {
            shortestPath[v].dist = shortestPath[u].dist + graph[u][v];
            previousVert[v] = u;
         }
   }

   reconstructPath(shortestPath, previousVert, src, vertCount);
   for (int i = 0; i < vertCount; i++)
   {
      printList(shortestPath[i].path);
   }
   free(sptSet);
   free(previousVert);
   sptSet = NULL;
   previousVert = NULL;
   
   return shortestPath;
}