#include <hashtable_module/hashtable.h>
#include <nodelist_module/nodelist.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static size_t hash_function(const char* key, size_t hashTableSize);
static void handle_collision(HashTable* hashTable, size_t index, Record* record);

static size_t hash_function(const char* key, size_t hashTableSize)
{
    if (key == NULL)
    {
        return (size_t)-1; // todo think about it could be better
    }

    size_t i = 0;

    for (size_t j = 0; key[j]; j++)
    {
        i += (size_t)key[j];
    }

    return i % hashTableSize;
}

Record* record_new(const char* key, const char* value)
{
    if (key == NULL || value == NULL)
    {
        return NULL;
    }

    Record* record = malloc(sizeof(Record));
    if (record == NULL)
    {
        return NULL;
    }

    record->key = key;
    record->value = value;
    
    return record;
}

void record_delete(Record* record)
{
    if (record == NULL)
    {
        return;
    }

    free(record);
}

HashTable* hashTable_new(size_t size)
{
    if (size < 1)
    {
        return NULL;
    }

    HashTable* hashTable = malloc(sizeof(HashTable));
    if (hashTable == NULL)
    {
        return NULL;
    }

    hashTable->size = size;
    hashTable->noOfElems = 0;

    hashTable->records = calloc(hashTable->size, sizeof(Record*));
    if (hashTable->records == NULL)
    {
        free(hashTable);
        return NULL;
    }

    hashTable->collisionList = calloc(hashTable->size, sizeof(NodeList*));
    if (hashTable->collisionList == NULL)
    {
        free(hashTable->records);
        free(hashTable);
        return NULL;
    }

    return hashTable;
}

void hashTable_delete(HashTable* hashTable)
{
    if (hashTable == NULL)
    {
        return;
    }

    for (size_t i = 0; i < hashTable->size; ++i)
    {
        if (hashTable->records[i] != NULL)
        {
            record_delete(hashTable->records[i]);
        }

        if (hashTable->collisionList[i] != NULL)
        {
            nodeList_delete(&hashTable->collisionList[i]);
        }
    }

    if (hashTable->records != NULL)
    {
        free(hashTable->records);
    }

    if (hashTable->collisionList != NULL)
    {
        free(hashTable->collisionList);
    }

    free(hashTable);
}

void hashTable_print(HashTable* hashTable)
{
    if (hashTable == NULL)
    {
        return;
    }

    for (size_t i = 0; i < hashTable->size; i++)
    {
        if (hashTable->records[i] != NULL)
        {
            printf("Index=%lu\tKey: %s, Value: %s\n",
                    i, 
                    hashTable->records[i]->key,
                    hashTable->records[i]->value);
        }

        if (hashTable->collisionList[i] != NULL)
        {
            NodeList* current = hashTable->collisionList[i];
            while (current != NULL)
            {
                Record* rec = (Record*)current->data;
                printf("\tKey: %s, Value: %s\n", rec->key, rec->value);
                current = current->next;
            }
        }
    }
}

void hashTable_insert(HashTable* hashTable, const char* key, const char* value)
{
    if (hashTable == NULL || key == NULL || value == NULL)
    {
        return;
    }

    // hashTable is full
    if (hashTable->size == hashTable->noOfElems)
    {
        return;
    }

    Record* record = record_new(key, value);
    if (record == NULL)
    {
        return;
    }

    size_t index = hash_function(key, hashTable->size);

    if (hashTable->records[index] == NULL)
    {
        // insert new record to hashTable
        hashTable->records[index] = record;
        hashTable->noOfElems++;
    }
    else
    {
        // if there is a same key, just update data
        if (strcmp(key, hashTable->records[index]->key) == 0)
        {
            hashTable->records[index]->value = value;
        }
        // in other case we got the collision (different keys gives same index)
        else
        {
            handle_collision(hashTable, index, record);
            hashTable->noOfElems++;
        }
    }
}

void hashTable_delete_record(HashTable* hashTable, const char* key)
{
    if (hashTable == NULL || key == NULL || hashTable->noOfElems < 1)
    {
        return;
    }

    const size_t index = hash_function(key, hashTable->size);
    if (hashTable->records[index] == NULL)
    {
        return;
    }

    // Record with given key has been found in the main hashtable records array
    if (strcmp(hashTable->records[index]->key, key) == 0)
    {
        record_delete(hashTable->records[index]);
        hashTable->records[index] = NULL;
        hashTable->noOfElems--;
        return;
    }
    // Index generated from key exists, but the key doesn't match.
    // Check if we have a collision list.
    else if (hashTable->collisionList[index] != NULL)
    {
        NodeList* head = hashTable->collisionList[index];
        NodeList* current = head;
        Record* record = current->data;

        // Search until the element is found or the list ends
        while (current != NULL && strcmp(record->key, key) != 0)
        {
            current = current->next;
            if (current != NULL)
            {
                record = current->data;
            }
        }

        // Element not found
        if (current == NULL)
        {
            return;
        }
        // If deleted element is the head of the list
        else if (current == head)
        {
            hashTable->collisionList[index] = current->next;
            nodeList_node_delete(&head, record);
            hashTable->noOfElems--;
        }
        // If deleted element is not the head of the list
        else
        {
            nodeList_node_delete(&head, record);
            hashTable->noOfElems--;
        }
    }
    // Key not found
    else
    {
        return;
    }
}

char* hashTable_search(HashTable* hashTable, const char* key)
{
    if (hashTable == NULL || key == NULL)
    {
        return NULL;
    }

    size_t index = hash_function(key, hashTable->size);

    if (index == (size_t)-1)
    {
        return NULL;
    }

    if (hashTable->records[index] == NULL)
    {
        return NULL;
    }

    if (strcmp(hashTable->records[index]->key, key) == 0)
    {
        return (char*) hashTable->records[index]->value;
    }
    else // collision case
    {
        NodeList* head = hashTable->collisionList[index];
        Record* record = NULL;
        while (head != NULL)
        {
            record = head->data;
            if (strcmp(record->key, key) == 0)
            {
                return (char*) record->value;
            }
            head = head->next;
        }
        return NULL;
    }
}

static void handle_collision(HashTable* hashTable, size_t index, Record* record)
{
    if (hashTable == NULL || record == NULL)
    {
        return;
    }

    NodeList* head = hashTable->collisionList[index];

    // if list does not exist yet
    if (head == NULL)
    {
        // Creates the list.
        head = nodeList_new(record);
        hashTable->collisionList[index] = head;
        return;
    }

    // Insert to the list.
    nodeList_insert(&head, record);
    hashTable->collisionList[index] = head;
}

