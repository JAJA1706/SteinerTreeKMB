#ifndef LIST_H
#define LIST_H

typedef struct Node
{
   int val;
   struct Node* next;
} Node;

typedef struct IntList
{
   Node* head;
} IntList;

void push_back(IntList* list, int val);
int get(IntList list, int index);
void deleteList(IntList* list);
void printList(IntList list);
int size(IntList list);

#endif //LIST_H