#include <nodelist_module/nodelist.h>
#include <stdlib.h>

NodeList* nodeList_new(void* data)
{
    if (data == NULL)
    {
        return NULL;
    }

    NodeList* head = malloc(sizeof(NodeList));
    if (head == NULL)
    {
        return NULL;
    }

    head->data = data;
    head->next = NULL;

    return head;
}

// if you want to delete whole list just remember not to frees specific data inserted in list 
void nodeList_delete(NodeList** head)
{
    if (*head == NULL)
    {
        return;
    }

    NodeList* current = *head;
    NodeList* next = NULL;

    while (current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    *head = NULL;    
}

// double pointer to NodeList since we want to overwright the head
void nodeList_insert(NodeList** head, void* data)
{
    if (*head == NULL || data == NULL)
    {
        return;
    }

    NodeList* newNode = nodeList_new(data);
    if (newNode == NULL)
    {
        return;
    }

    // assing previous node(head) to the newNode pointer to the next
    newNode->next = *head;

    // update current head of the list (newNode will be 1st element)
    *head = newNode;
}

void nodeList_node_delete(NodeList** head, void* data)
{
    if (*head == NULL || data == NULL)
    {
        return;
    }

    NodeList* currentNode = *head;
    NodeList* previousNode = NULL;
    
    // Define searched node as a currentNode and specify previousNode
    while (currentNode != NULL && currentNode->data != data)
    {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    // Can't find that node
    if (currentNode == NULL)
    {
        return;
    }

    // Case when searched element is the head
    if (previousNode == NULL)
    {
        *head = currentNode->next;
    }
    // Case when searched element is not the head
    else
    {
        previousNode->next = currentNode->next;
    }

    free(currentNode->data);
    free(currentNode);
    currentNode = NULL;
}

/*
void nodeList_printNodes(NodeList* head)
{
    if (head == NULL)
    {
        return;
    }

    NodeList* headCopy = head;

    while(headCopy != NULL)
    {
        printf("%s\n", headCopy->record->value);
        headCopy = headCopy->next;
    } 
}
*/