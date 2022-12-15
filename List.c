#include "List.h"
#include <stdlib.h>
#include <stdio.h>
 
void push_back(IntList* list, int val)
{
   Node* current = list->head;
   Node* previous = NULL;
   while( current != NULL )
   {
      previous = current;
      current = current->next;
   }

   current = malloc(sizeof(Node));
   if( previous != NULL )
   {
      previous->next = current;
   }
   else
   {
      list->head = current;
   }

   current->val = val;
   current->next = NULL;
}

int get(IntList list, int index)
{
   Node* current = list.head;
   for(int i = 0; current != NULL; ++i)
   {
      if(i == index)
      {
         return current->val;
      }
      current = current->next;
   }
   return -9999;
}

void deleteList(IntList* list)
{
   Node* current = list->head;
   Node* previous;
   while(current != NULL)
   {
      previous = current;
      current = current->next;
      free(previous);
      previous = NULL;
   }
}

void printList(IntList list)
{
   Node* current = list.head;
   while(current != NULL)
   {
      printf("%d ", current->val);
      current = current->next;
   }

   printf("\n");
}

int size(IntList list)
{
   Node* current = list.head;
   int size = 0;
   while(current != NULL)
   {
      ++size;
      current = current->next;
   }

   return size;
}