#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileProcess.h"
#include "steiner.h"
#include "List.h"

void free2dPaths(Path** path, int dim)
{
    for(int i = 0; i < dim; ++i)
    {
        deleteList(&path[i]->path);
        free(path[i]);
    }
    free(path);
}

void free2dArray(int** array, int dim)
{
   for(int i = 0; i < dim; ++i)
   {
      free(array[i]);
   }
   free(array);
}

int main(int argc, char** argv) {
   Graph graph = {0, NULL};
   graph.values = readGraphFromFile("F:\\source\\PORR\\data\\graph.txt", &graph.vertNum);

   int steinerVertNum = 0;
   int* steinerVerts = readSteinerVerticesFromFile("F:\\source\\PORR\\data\\SteinerVecs.txt", &steinerVertNum);

   Path** shortestPathsFromSteinerVerts = calculateShortestPathsFromSteinerVerts(graph, steinerVerts, steinerVertNum);

   Graph completeShortestGraph;
   completeShortestGraph.vertNum = steinerVertNum;
   completeShortestGraph.values = constructCompleteShortestGraphWithSteinerVecs(shortestPathsFromSteinerVerts, steinerVerts, steinerVertNum); //STEP 1

   MSTEdges* firstMSTEdges = getEdgesOfMinimalSpanningTree(completeShortestGraph); //STEP 2

   reconstructTreeByLeavingOnlyMSTEdges(&graph, firstMSTEdges, shortestPathsFromSteinerVerts, steinerVertNum); //STEP 3

   MSTEdges* secondMSTEdges = getEdgesOfMinimalSpanningTree(graph); //STEP 4

   reconstructTreeToSteinerTree(&graph, secondMSTEdges, steinerVerts, steinerVertNum); //STEP 5

   free(steinerVerts);
   free2dArray(graph.values, graph.vertNum);
   free2dArray(completeShortestGraph.values, completeShortestGraph.vertNum);
   free2dPaths(shortestPathsFromSteinerVerts, steinerVertNum);
   free(firstMSTEdges);
   free(secondMSTEdges);
   return 0;
}