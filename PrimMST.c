#include "PrimMST.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int minKey(int* key, bool* mstSet, int vertNum)
{
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < vertNum; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
 
    return min_index;
}
 
int printMST(int* parent, const Graph graph)
{
    printf("Edge \tWeight\n");
    for (int i = 1; i < graph.vertNum; i++)
        printf("%d - %d \t%d \n", parent[i]+1, i+1,
               graph.values[i][parent[i]]);
}

//returns dynamically allocated edges of MST tree
MSTEdges* primMST(const Graph graph)
{
    int* parent = malloc(sizeof(int)*graph.vertNum);
    int* key = malloc(sizeof(int)*graph.vertNum);
    bool* mstSet = malloc(sizeof(bool)*graph.vertNum);
 
    for (int i = 0; i < graph.vertNum; i++)
        key[i] = INT_MAX, mstSet[i] = false;
 
    key[0] = 0;
    parent[0] = -1;
 
    for (int count = 0; count < graph.vertNum - 1; count++) {
        int u = minKey(key, mstSet, graph.vertNum);
        mstSet[u] = true;
        for (int v = 0; v < graph.vertNum; v++)
            if (graph.values[u][v] && mstSet[v] == false
                && graph.values[u][v] < key[v])
                parent[v] = u, key[v] = graph.values[u][v];
    }
 
    const int mstEdgesCount = graph.vertNum - 1;
    MSTEdges* edges = malloc(sizeof(MSTEdges)*(mstEdgesCount));
    for(int i = 0; i < mstEdgesCount; ++i)
    {
        edges[i].first = parent[i+1];
        edges[i].second = i+1;
    }


    printMST(parent, graph);

    free(parent);
    free(key);
    free(mstSet);
    key = NULL;
    mstSet = NULL;
    return edges;
}