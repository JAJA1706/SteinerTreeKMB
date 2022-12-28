#ifndef PARALLEL_H
#define PARALLEL_H
#include <pthread.h>
#include "Steiner.h"

typedef struct readGraphArgs
{
   int*** graph;
   char* fileName;
   int* verticesCount;
} readGraphArgs;

typedef struct readSteinerArgs
{
   int** steinerVerts;
   int* steinerVertNum;
   char* fileName;
} readSteinerArgs;

pthread_t readGraphFromFileParallel(Graph* graph, char* fileName);
pthread_t readSteinerVerticesFromFileParallel(int** steinerVerts, int* steinerVertNum, char* fileName);

#endif //PARALLEL_H