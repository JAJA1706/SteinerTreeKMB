#include "steiner.h"
#include "PrimMST.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
 
void printGraph(const Graph graph)
{
    for(int i = 0; i < graph.vertNum; ++i)
    {
        for(int j = 0; j < graph.vertNum; ++j)
        {
            printf("%d ", graph.values[i][j]);
        }
        printf("\n");
    }
}

 //returns dynamically allocated paths
Path** calculateShortestPathsFromSteinerVerts(const Graph graph, const int* steinerVerts, const int steinerVertsNum)
{
    Path** shortestPaths = malloc(sizeof(Path*)*steinerVertsNum);
    for(int i = 0; i < steinerVertsNum; ++i)
    {
        Path* path = dijkstra(graph.values, steinerVerts[i], graph.vertNum);
        shortestPaths[i] = path;
    }

   return shortestPaths;
}

 //returns dynamically allocated graph
int** constructCompleteShortestGraphWithSteinerVecs(Path** const shortestPathsFromSteinerVerts, const int* steinerVerts, const int steinerVertsNum)
{
    int** completeShortestGraph = NULL;
    completeShortestGraph = malloc(sizeof(int*) * steinerVertsNum);
    for(int i = 0; i < steinerVertsNum; ++i)
    {
        completeShortestGraph[i] = malloc(sizeof(int) * steinerVertsNum);
    }

    for(int i = 0; i < steinerVertsNum; ++i)
    {
        for(int j = 0; j < steinerVertsNum; ++j)
        {
            completeShortestGraph[i][j] = shortestPathsFromSteinerVerts[i][ steinerVerts[j] ].dist;
        }
    }

    return completeShortestGraph;
}

MSTEdges* getEdgesOfMinimalSpanningTree(const Graph graphToCalculateMST)
{
     return primMST(graphToCalculateMST);
}

void reconstructTreeByLeavingOnlyMSTEdges(Graph* graph, MSTEdges* mstEdges, Path** shortestPaths, const int steinerVertsNum)
{
    bool** includedVals = malloc(sizeof(bool*)*graph->vertNum);
    for(int i = 0; i < graph->vertNum; ++i)
        includedVals[i] = malloc(sizeof(bool)*graph->vertNum);

    for(int i = 0; i < graph->vertNum; ++i)
        for(int j = 0; j < graph->vertNum; ++j)
            includedVals[i][j] = false;


    const int mstEdgesCount = steinerVertsNum - 1;
    for(int i = 0; i < mstEdgesCount; ++i)
    {
        int startVert = mstEdges[i].first;
        int endVert = mstEdges[i].second;
        Path includedPath;
        for(int j = 0; j < graph->vertNum; ++j)
        {
            if(get(shortestPaths[startVert][j].path, 0) == endVert)
            {
                printList(shortestPaths[startVert][j].path);
                includedPath = shortestPaths[startVert][j];
                break;
            }
        }

        for(int j = 0; j < size(includedPath.path) - 1; ++j)
        {
            int vert = get(includedPath.path, j);
            int vertNext = get(includedPath.path, j+1);
            includedVals[vert][vertNext] = true;
            includedVals[vertNext][vert] = true;
        }
    }

    for(int i = 0; i < graph->vertNum; ++i)
        for(int j = 0; j < graph->vertNum; ++j)
            if(includedVals[i][j] == false)
                graph->values[i][j] = 0;

    removeUnnecessaryVertices(graph);
    printGraph(*graph);

    for(int i = 0; i < graph->vertNum; ++i)
        free(includedVals[i]);
    free(includedVals);
}

void reconstructTreeToSteinerTree(Graph* graph, MSTEdges* mstEdges, const int* steinerVerts, const int steinerVertsNum)
{
    bool** includedVals = malloc(sizeof(bool*)*graph->vertNum);
    for(int i = 0; i < graph->vertNum; ++i)
        includedVals[i] = malloc(sizeof(bool)*graph->vertNum);

    for(int i = 0; i < graph->vertNum; ++i)
        for(int j = 0; j < graph->vertNum; ++j)
            includedVals[i][j] = false;

    const int mstEdgesCount = graph->vertNum - 1;
    for(int i = 0; i < mstEdgesCount; ++i) //leave only mstedges
    {
        int vertX = mstEdges[i].first;
        int vertY = mstEdges[i].second;
        includedVals[vertX][vertY] = true;
        includedVals[vertY][vertX] = true;
    }

    for(int i = 0; i < graph->vertNum; ++i) //remove leaves which are not steinerVertices
    {
        bool isSteinerVert = false;
        for(int j = 0; j < steinerVertsNum; ++j)
        {
            if(steinerVerts[j] == i)
                isSteinerVert = true;
        }
        if(isSteinerVert)
        {
            continue;
        }

        int numOfConnects = 0;
        int* isConnectedWith;
        for(int j = 0; j < mstEdgesCount; ++j)
        {
            if(mstEdges[j].first == i)
            {
                isConnectedWith = &mstEdges[j].second;
                ++numOfConnects;
            }
            else if(mstEdges[j].second == i)
            {
                isConnectedWith = &mstEdges[j].first;
                ++numOfConnects;
            }
        }

        if(numOfConnects < 2)
        {
            includedVals[i][*isConnectedWith] = false;
            includedVals[*isConnectedWith][i] = false;
            if(i > *isConnectedWith)
            {
                i = *isConnectedWith;
            }
            *isConnectedWith = -1; //disconnecting
        }
    }

    for(int i = 0; i < graph->vertNum; ++i)
        for(int j = 0; j < graph->vertNum; ++j)
            if(includedVals[i][j] == false)
                graph->values[i][j] = 0;

    removeUnnecessaryVertices(graph);
    printGraph(*graph);

    for(int i = 0; i < graph->vertNum; ++i)
        free(includedVals[i]);
    free(includedVals);
}

void removeUnnecessaryVertices(Graph* graph)
{
    int vertexNum = 1;
    for(int i = 0; i < graph->vertNum; ++i)
    {
        bool isRedundant = true;
        for(int j = 0; j < graph->vertNum; ++j)
        {
            if(graph->values[i][j] != 0)
            {
                isRedundant = false;
            }
        }

        if(isRedundant)
        {
            printf("Removing vertex %d\n", vertexNum);
            for(int x = 0; x < graph->vertNum-1; ++x)
            {
                for(int y = 0; y < graph->vertNum-1; ++y)
                {
                    if(x >= i && y >= i)
                    {
                        graph->values[x][y] = graph->values[x+1][y+1];
                    }
                    else if(y >= i)
                    {
                        graph->values[x][y] = graph->values[x][y+1];
                    }
                    else if(x >= i)
                    {
                        graph->values[x][y] = graph->values[x+1][y];
                    }
                }
            }

            free(graph->values[graph->vertNum-1]); //we should only deinitialize last row, last column will be left unused
            graph->values[graph->vertNum-1] = NULL;
            --graph->vertNum;
            --i;
        }
        ++vertexNum;
    }
}
