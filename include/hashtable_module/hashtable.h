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
    NodeList** collisionList; // array of pointers to heads of the node lists
} HashTable;


Record* record_new(const char* key, const char* value);
void record_delete(Record* record);

HashTable* hashTable_new(const size_t size);
void hashTable_delete(HashTable* hashTable);

void hashTable_print(const HashTable* hashTable);
void hashTable_insert(HashTable* hashTable, const char* key, const char* value);
void hashTable_delete_record(HashTable* hashTable, const char* key);
const char* hashTable_search(const HashTable* hashTable, const char* key);

#endif // HASHTABLE_H