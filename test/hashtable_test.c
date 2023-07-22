#include <hashtable.c>
#include <nodelist.c>
#include <assert.h>
#include <stdio.h>

void hashtable_record_new_test(void);
void hashtable_new_test(void);
void hashtable_insert_test(void);
void hashtable_hash_function_test(void);
void hashtable_collision_test(void);

void hashtable_record_new_test(void)
{
    // incorrect key, should return NULL
    {
        const char* key = NULL;
        const char* val = "example";
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    // incorrect value, should return NULL
    {
        const char* key = "someKey";
        const char* val = NULL;
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    // incorrect both arguments, should return NULL
    {
        const char* key = NULL;
        const char* val = NULL;
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    //  both arguments correct, check if record is properly filled
    {
        const char* key = "exampKey";
        const char* val = "someValue";
        Record* record = record_new(key, val);

        assert(record != NULL);
        assert(record->key == key);
        assert(record->value == val);

        record_delete(record);
    }
}

void hashtable_new_test(void)
{
    // passed size is 0, so hash table should not be created (return NULL)
    {
        HashTable* hashTable = hashTable_new(0);
        assert(hashTable == NULL);
    }

    // correct argument, check if eveything has been properly filled
    {
        size_t table_size = 5;
        HashTable* hashTable = hashTable_new(table_size);

        assert(hashTable != NULL);
        assert(hashTable->size == table_size);
        assert(hashTable->noOfElems == 0);
        for (size_t i = 0; i < table_size; ++i)
        {
            assert(hashTable->records[i] == NULL);
            assert(hashTable->collisionList[i] == NULL);
        }

        hashTable_delete(hashTable);
    }
}

void hashtable_insert_test(void)
{

    // check incorrect argument (bad key)
    {
        size_t table_size = 1;
        const char* key = NULL;
        const char* val = "valOne";
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_insert(ht, key, val);     
  
        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);  
    }

    // check incorrect argument (bad value)
    {
        size_t table_size = 1;
        const char* key = "keyOne";
        const char* val = NULL;
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_insert(ht, key, val);     
  
        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);  
    }

    // check correct insertion of one element
    {
        size_t table_size = 1;
        const char* key = "keyOne";
        const char* val = "valOne";
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_insert(ht, key, val);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 1);
        assert(ht->records[0] != NULL);
        assert(ht->records[0]->key == key);
        assert(ht->records[0]->value == val);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);
    }

    // check correct insertion of two elements
    {
        size_t table_size = 10;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);

        hashTable_insert(ht, key, val);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 1);
    
        hashTable_insert(ht, key2, val2);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 2);

        hashTable_delete(ht);
    }
}

void hashtable_hash_function_test(void)
{
    // Add 3 records to 50-elemets hashTable ande verify if hashFunction returns the proper index
    {
        size_t table_size = 50;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        const char* key3 = "keyThree";
        const char* val3 = "valThree";
        size_t index = 0;
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);

        hashTable_insert(ht, key, val);

        index = hash_function(key, ht->size);
        assert(ht->records[index] != NULL);
        assert(ht->records[index]->key == key);
        assert(ht->records[index]->value == val);
    
        hashTable_insert(ht, key2, val2);

        index = hash_function(key2, ht->size);
        assert(ht->records[index] != NULL);
        assert(ht->records[index]->key == key2);
        assert(ht->records[index]->value == val2);

        hashTable_insert(ht, key3, val3);

        index = hash_function(key3, ht->size);
        assert(ht->records[index] != NULL);
        assert(ht->records[index]->key == key3);
        assert(ht->records[index]->value == val3);

        hashTable_delete(ht);        
    }
}

void hashtable_collision_test(void)
{
    // this set of input data will force collision
    {
        size_t table_size = 2;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        size_t index = 3;
        size_t index2 = 5;
        HashTable* ht = hashTable_new(table_size);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->records[1] == NULL);
        assert(ht->collisionList[0] == NULL);
        assert(ht->collisionList[1] == NULL);

        hashTable_insert(ht, key, val);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 1);
        index = hash_function(key, ht->size);
        assert(ht->records[index] != NULL);
        assert(ht->records[index]->key == key);
        assert(ht->records[index]->value == val);
        assert(ht->collisionList[0] == NULL);
        assert(ht->collisionList[1] == NULL);

        hashTable_insert(ht, key2, val2);

        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 2);
        index2 = hash_function(key2, ht->size);
        assert(index == index2);
        assert(ht->records[index2] != NULL);
        assert(ht->records[index2]->key == key);
        assert(ht->records[index2]->value == val);

        assert(ht->collisionList[index2] != NULL);
        // @todo verify adding to handling list

        hashTable_delete(ht);
    }
}