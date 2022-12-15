#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_NUM_OF_VERTS 32
#define MAX_EDGE_LENGTH 32

int calculateBufferSizeForFileReading(const int maxNumbersInRow, const int maxNumberValue)
{
    int maxCharsToSpecifyNumber = 0;
    int x = maxNumberValue;
    while( x != 0)
    {
        x /= 10;
        ++maxCharsToSpecifyNumber;
    }

    return maxNumbersInRow * maxCharsToSpecifyNumber + maxNumbersInRow; //+maxNumbersInRow because of spaces
}

//returns dynamically allocated array, expects already allocated verticesCount
int** readGraphFromFile(char* fileName, int* verticesCount)
{
    int** graph = NULL;

    const int BUFF_SIZE = calculateBufferSizeForFileReading(MAX_NUM_OF_VERTS, MAX_EDGE_LENGTH); 
    char* buff = malloc(BUFF_SIZE);
    FILE* file = fopen(fileName, "r");
    if(!file)
    {
        puts("Couldn't open file with graph definition.");
        return graph;
    }

    fgets(buff, BUFF_SIZE, file);
    char* str = strtok(buff, " ");
    if(str == NULL)
    {
        return graph;
    }

    int vertCount = 0;
    while(str != NULL)
    {
        str = strtok(NULL, " ");
        ++vertCount;
    }


    graph = malloc(sizeof(int*)*vertCount);
    for(int i = 0; i < vertCount; ++i)
    {
        graph[i] = malloc(sizeof(int)*vertCount);
    }

    fseek(file, 0, 0);

    int edgeInVert = 0;
    int vert = 0;
    while( !feof(file) && vert < vertCount )
    {
        fgets(buff, BUFF_SIZE, file);
        char* numOrEndline = strtok(buff, " ");
        int edge = atoi(numOrEndline);
        graph[vert][edgeInVert] = edge;
        ++edgeInVert;
        while(numOrEndline != NULL && edgeInVert < vertCount)
        {
            numOrEndline = strtok(NULL, " ");
            int edge = atoi(numOrEndline);
            graph[vert][edgeInVert] = edge;
            ++edgeInVert;
        }
            ++vert;
            edgeInVert = 0;
    }

    free(buff);
    *verticesCount = vertCount;
    return graph;
}

//returns dynamically allocated array, expects already allocated verticesCount
int* readSteinerVerticesFromFile(char* fileName, int* verticesCount)
{
    int* steinerVerts = NULL;

    const int BUFF_SIZE = calculateBufferSizeForFileReading(MAX_NUM_OF_VERTS, MAX_NUM_OF_VERTS);
    char* buff = malloc(BUFF_SIZE);
    FILE* file = fopen(fileName, "r");
    if(!file)
    {
        puts("Couldn't open file with steiner verttors definition.");
        return steinerVerts;
    }

    fgets(buff, BUFF_SIZE, file);
    int verts[MAX_NUM_OF_VERTS] = {0};
    char* str = strtok(buff, " ");
    if(str == NULL)
    {
        return steinerVerts;
    }

    verts[0] = atoi(str);
    int vertCount = 0;
    for(int i = 1; str != NULL && i < MAX_NUM_OF_VERTS; ++i)
    {
        str = strtok(NULL, " ");
        verts[i] = atoi(str);
        ++vertCount;
    }
    
    steinerVerts = malloc(sizeof(int) * vertCount);
    for(int i = 0; i < vertCount; ++i)
    {
        steinerVerts[i] = verts[i];
    }

    free(buff);
    *verticesCount = vertCount;
    return steinerVerts;
}