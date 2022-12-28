#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "FileProcess.h"
#include "Steiner.h"
#include "List.h"
#include "ParallelFunctions.h"
#include <omp.h>

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
   omp_set_dynamic(1);

   Graph graph = {0, NULL};
   pthread_t threadReadFile1 = readGraphFromFileParallel(&graph, "F:\\source\\PORR\\data\\graphXXL.txt");
   int steinerVertNum = 0;
   int* steinerVerts = NULL;
   pthread_t threadReadFile2 = readSteinerVerticesFromFileParallel(&steinerVerts, &steinerVertNum, "F:\\source\\PORR\\data\\SteinerVertXXL.txt");

   pthread_join(threadReadFile1, NULL);
   pthread_join(threadReadFile2, NULL);

   clock_t timer = clock();

   Path** shortestPathsFromSteinerVerts = calculateShortestPathsFromSteinerVerts(graph, steinerVerts, steinerVertNum);

   Graph completeShortestGraph;
   completeShortestGraph.vertNum = steinerVertNum;
   completeShortestGraph.values = constructCompleteShortestGraphWithSteinerVecs(shortestPathsFromSteinerVerts, steinerVerts, steinerVertNum); //STEP 1

   MSTEdges* firstMSTEdges = getEdgesOfMinimalSpanningTree(completeShortestGraph); //STEP 2

   reconstructTreeByLeavingOnlyMSTEdges(&graph, firstMSTEdges, shortestPathsFromSteinerVerts, steinerVerts, steinerVertNum); //STEP 3

   MSTEdges* secondMSTEdges = getEdgesOfMinimalSpanningTree(graph); //STEP 4

   reconstructTreeToSteinerTree(&graph, secondMSTEdges, steinerVerts, steinerVertNum); //STEP 5

   printf("Time: %ld", clock() - timer);

   free(steinerVerts);
   free2dArray(graph.values, graph.vertNum);
   free2dArray(completeShortestGraph.values, completeShortestGraph.vertNum);
   free2dPaths(shortestPathsFromSteinerVerts, steinerVertNum);
   free(firstMSTEdges);
   free(secondMSTEdges);
   return 0;
}