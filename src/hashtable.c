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

    for (size_t i = 0; i < hashTable->size; ++i)
    {
        hashTable->records[i] = NULL;
        hashTable->collisionList[i] = NULL;
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
        if (hashTable->records[i] == NULL)
        {
            continue;
        }
        
        printf("Index=%lu, key=%s, value=%s\n",
                i, 
                hashTable->records[i]->key,
                hashTable->records[i]->value);
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

