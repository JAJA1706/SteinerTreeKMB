#include "ParallelFunctions.h"
#include <stdlib.h>
#include "FileProcess.h"
 
void* readGraphThread(void* _args)
{
   readGraphArgs* args = (readGraphArgs*)_args;
   *(args->graph) = readGraphFromFile(args->fileName, args->verticesCount);
   free(_args);
   _args = NULL;
}

pthread_t readGraphFromFileParallel(Graph* graph, char* fileName)
{
   readGraphArgs* args = malloc(sizeof(readGraphArgs));
   args->graph = &graph->values;
   args->fileName = fileName;
   args->verticesCount = &graph->vertNum;

   pthread_t thread_id;
   pthread_create(&thread_id, NULL, readGraphThread, args);
   return thread_id;
}

void* readSteinerVerticesThread(void* _args)
{
   readSteinerArgs* args = (readSteinerArgs*)_args;
   *(args->steinerVerts) = readSteinerVerticesFromFile(args->fileName, args->steinerVertNum);
   free(_args);
   _args = NULL;
}

pthread_t readSteinerVerticesFromFileParallel(int** steinerVerts, int* steinerVertNum, char* fileName)
{
   readSteinerArgs* args = malloc(sizeof(readSteinerArgs));
   args->steinerVerts = steinerVerts;
   args->steinerVertNum = steinerVertNum;
   args->fileName = fileName;

   pthread_t thread_id;
   pthread_create(&thread_id, NULL, readSteinerVerticesThread, args);
   return thread_id;
}