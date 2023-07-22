#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>
#include <nodelist_module/nodelist.h>

typedef struct Record
{
    const char* key;
    const char* value;
} Record;

typedef struct HashTable
{
    size_t size; // size of the hash table
    size_t noOfElems; // number of elements in hash table
    Record** records; // array of pointers to records
    NodeList** collisionList;
} HashTable;


Record* record_new(const char* key, const char* value);
void record_delete(Record* record);

HashTable* hashTable_new(size_t size);
void hashTable_delete(HashTable* hashTable);

void hashTable_print(HashTable* hashTable);
void hashTable_insert(HashTable* hashTable, const char* key, const char* value);
char* hashTable_search(HashTable* hashTable, const char* key);

#endif // HASHTABLE_H