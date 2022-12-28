#ifndef STEINER_H
#define STEINER_H
#include "Dijkstra.h"

typedef struct MSTEdges MSTEdges;

typedef struct Graph
{
   int vertNum;
   int** values;
} Graph;

void removeUnnecessaryVertices(Graph* graph);
void printGraph(const Graph graph);
Path** calculateShortestPathsFromSteinerVerts(const Graph graph, const int* steinerVerts, const int steinerVertsNum);
int** constructCompleteShortestGraphWithSteinerVecs(Path** const shortestPathsFromSteinerVerts, const int* steinerVerts, const int steinerVertsNum);
MSTEdges* getEdgesOfMinimalSpanningTree(const Graph graphToCalculateMST);
void reconstructTreeByLeavingOnlyMSTEdges(Graph* graph, MSTEdges* mstEdges, Path** shortestPaths, const int* steinerVerts, const int steinerVertsNum);
void reconstructTreeToSteinerTree(Graph* graph, MSTEdges* mstEdges, const int* steinerVerts, const int steinerVertsNum);

#endif //STEINER_H