#ifndef NODELIST_H
#define NODELIST_H

#include <stddef.h>

typedef struct NodeList
{
    void* data;
    struct NodeList* next;
} NodeList;


NodeList* nodeList_new(void* data); 
void nodeList_delete(NodeList** head);
void nodeList_insert(NodeList** head, void* data);
void nodeList_node_delete(NodeList** head, void* data);


#endif // NODELIST_H
