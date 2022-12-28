#ifndef PRIMMST_H
#define PRIMMST_H
#include "Steiner.h"

typedef struct MSTEdges
{
    int first;
    int second;
} MSTEdges;

int printMST(int* parent, const Graph graph);
MSTEdges* primMST(const Graph graph);

#endif //PRIMMST_H