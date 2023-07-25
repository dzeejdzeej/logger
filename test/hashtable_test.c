#include <hashtable.c>
#include <nodelist.c>
#include <assert.h>
#include <stdio.h>
#include <string.h>

void hashtable_record_new_test(void);
void hashtable_new_test(void);
void hashtable_insert_test(void);
void hashtable_hash_function_test(void);
void hashtable_collision_test(void);
void hashtable_search_test(void);

// Test function: Record* record_new(const char* key, const char* value);
void hashtable_record_new_test(void)
{
    // Incorrect key, should return NULL
    {
        const char* key = NULL;
        const char* val = "example";
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    // Incorrect value, should return NULL
    {
        const char* key = "someKey";
        const char* val = NULL;
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    // Incorrect both arguments, should return NULL
    {
        const char* key = NULL;
        const char* val = NULL;
        Record* record = record_new(key, val);

        assert(record == NULL);
    }

    // Both arguments correct, check if record is properly filled
    {
        const char* key = "exampKey";
        const char* val = "someValue";
        Record* record = record_new(key, val);

        assert(record != NULL);
        assert(strcmp(record->key, key) == 0);
        assert(strcmp(record->value, val) == 0);

        record_delete(record);
    }
}

// Test function: HashTable* hashTable_new(size_t size);
void hashtable_new_test(void)
{
    // Passed size is 0, so hash table should not be created (return NULL)
    {
        const size_t invalid_table_size = 0;
        HashTable* hashTable = hashTable_new(invalid_table_size);
        assert(hashTable == NULL);
    }

    // Correct argument, check if eveything has been properly filled
    {
        const size_t table_size = 5;
        HashTable* hashTable = hashTable_new(table_size);

        assert(hashTable != NULL);
        assert(hashTable->size == table_size);
        assert(hashTable->noOfElems == 0);

        // Hash table created but empty
        for (size_t i = 0; i < table_size; ++i)
        {
            assert(hashTable->records[i] == NULL);
            assert(hashTable->collisionList[i] == NULL);
        }

        hashTable_delete(hashTable);
    }
}

// Test function: void hashTable_insert(HashTable* hashTable, const char* key, const char* value);
void hashtable_insert_test(void)
{
    // Check incorrect argument (bad key)
    {
        const size_t table_size = 1;
        const char* key = NULL;
        const char* val = "valOne";

        HashTable* ht = hashTable_new(table_size);
        hashTable_insert(ht, key, val);     
  
        // Hash table created but still 0 elements since record data is incorrect due to NULL key
        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);  
    }

    // Check incorrect argument (bad value)
    {
        const size_t table_size = 1;
        const char* key = "keyOne";
        const char* val = NULL;

        HashTable* ht = hashTable_new(table_size);
        hashTable_insert(ht, key, val);     
  
        // Hash table created but still 0 elements since record data is incorrect due to NULL value
        assert(ht != NULL);
        assert(ht->size == table_size);
        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);  
    }

    // Check correct insertion of one element
    {
        const size_t table_size = 1;
        const char* key = "keyOne";
        const char* val = "valOne";
        HashTable* ht = hashTable_new(table_size);

        assert(ht->noOfElems == 0);
        assert(ht->records[0] == NULL);
        assert(ht->collisionList[0] == NULL);

        hashTable_insert(ht, key, val);

        // Check if element has been added to hash table
        assert(ht->noOfElems == 1);
        assert(ht->records[0] != NULL);
        assert(strcmp(ht->records[0]->key, key) == 0);
        assert(strcmp(ht->records[0]->value, val) == 0);
        assert(ht->collisionList[0] == NULL);

        hashTable_delete(ht);
    }

    // Check correct insertion of two elements
    {
        const size_t table_size = 10;
        size_t index = 0;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        HashTable* ht = hashTable_new(table_size);

        assert(ht->noOfElems == 0);

        hashTable_insert(ht, key, val);
        index = hash_function(key, ht->size);

        assert(ht->noOfElems == 1);
        assert(strcmp(ht->records[index]->key, key) == 0);
        assert(strcmp(ht->records[index]->value, val) == 0);
    
        hashTable_insert(ht, key2, val2);
        index = hash_function(key2, ht->size);

        assert(ht->noOfElems == 2);
        assert(strcmp(ht->records[index]->key, key2) == 0);
        assert(strcmp(ht->records[index]->value, val2) == 0);

        hashTable_delete(ht);
    }

    // Check incorrect second insertion - hash table size exceeded
    {
        const size_t table_size = 1;
        size_t index = 0;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        HashTable* ht = hashTable_new(table_size);

        assert(ht->noOfElems == 0);

        hashTable_insert(ht, key, val);
        index = hash_function(key, ht->size);

        assert(ht->noOfElems == 1);
        assert(ht->records[index] != NULL);
        assert(strcmp(ht->records[index]->key, key) == 0);
        assert(strcmp(ht->records[index]->value, val) == 0);
    
        hashTable_insert(ht, key2, val2);
        index = hash_function(key2, ht->size);
 
        // Check if new element has been rejected and old values remains
        assert(ht->noOfElems == 1);
        assert(ht->records[index] != NULL);
        assert(strcmp(ht->records[index]->key, key) == 0);
        assert(strcmp(ht->records[index]->value, val) == 0);

        hashTable_delete(ht);
    }
}

// Test function: static size_t hash_function(const char* key, size_t hashTableSize);
void hashtable_hash_function_test(void)
{
    // Add 3 records to 50-elemets hashTable ande verify if hashFunction returns the proper index
    {
        const size_t table_size = 50;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        const char* key3 = "keyThree";
        const char* val3 = "valThree";
        HashTable* ht = hashTable_new(table_size);

        hashTable_insert(ht, key, val);
        hashTable_insert(ht, key2, val2);
        hashTable_insert(ht, key3, val3);

        const size_t index  = hash_function(key, ht->size);
        const size_t index2 = hash_function(key2, ht->size);
        const size_t index3 = hash_function(key3, ht->size);

        // Confirm there is no collisions
        assert(index != index2);
        assert(index2 != index3);

        // Go through records array and check if proper records has been assigned
        for (size_t i = 0; i < table_size; ++i)
        {
            if (i != index && i != index2 && i != index3)
            {
                assert(ht->records[i] == NULL);
            }
            else
            {
                assert(ht->records[i] != NULL);
            }
        }

        hashTable_delete(ht);        
    }
}

// Verify collision handling within hashTable_insert() function
void hashtable_collision_test(void)
{
    // Add two records with different keys but which gave same index from hashing function
    {
        const size_t table_size = 2;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        size_t index = 3;
        size_t index2 = 5;
        HashTable* ht = hashTable_new(table_size);

        hashTable_insert(ht, key, val);
        index = hash_function(key, ht->size);

        // Check statistics after adding first one
        assert(ht->noOfElems == 1);
        assert(ht->records[index] != NULL);
        assert(strcmp(ht->records[index]->key, key) == 0);
        assert(strcmp(ht->records[index]->value, val) == 0);
        assert(ht->collisionList[0] == NULL);
        assert(ht->collisionList[1] == NULL);

        hashTable_insert(ht, key2, val2);
        index2 = hash_function(key2, ht->size);

        // Confirm that we have collision despites of using different keys
        assert(index == index2);
        assert(ht->records[index2] != NULL);

        // Two elements inserted despite of same indexes
        assert(ht->noOfElems == 2);

        // Previously written data should not be changed
        assert(strcmp(ht->records[index2]->key, key) == 0);
        assert(strcmp(ht->records[index2]->value, val) == 0);

        // But collision list should be now created
        assert(ht->collisionList[index2] != NULL);

        // New set of data should be added to collision list
        Record* rec2 = (Record*)ht->collisionList[index2]->data;
        NodeList* next = ht->collisionList[index2]->next;
        assert(rec2 != NULL);
        assert(strcmp(rec2->key, key2) == 0);
        assert(strcmp(rec2->value, val2) == 0);
        assert(next == NULL);

        hashTable_delete(ht);
    }

    /*  Add three records with different keys but which gave same index from hashing function.
        Since it's 3-element collision there is just one the same index for three different keys.
        To simplify, look at the test description below.
        * 1st insert
            records[index]                  = {key, val}
            collisionList[index]            = NULL
        * 2nd insert
            records[index]                  = {key, val}
            collisionList[index]            = 0x80
            collisionList[index].data       = {key2, val2}
            collisionList[index].next       = NULL
        * 3rd insert
            records[index]                  = {key, val}
            collisionList[index]            = 0x94
            collisionList[index].data       = {key3, val3}
            collisionList[index].next       = (0x80)
            collisionList[index].next.data  = {key2, val2}
            collisionList[index].next.next  = NULL
    */
    {
        const size_t table_size = 3;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        const char* key3 = "keyThr";
        const char* val3 = "valThr";
        size_t index = 3;
        size_t index2 = 5;
        size_t index3 = 0;
        HashTable* ht = hashTable_new(table_size);

        hashTable_insert(ht, key, val);
        index = hash_function(key, ht->size);

        // Check statistics after adding first one
        assert(ht->noOfElems == 1);
        assert(ht->records[index] != NULL);
        assert(strcmp(ht->records[index]->key, key) == 0);
        assert(strcmp(ht->records[index]->value, val) == 0);
        assert(ht->collisionList[0] == NULL);
        assert(ht->collisionList[1] == NULL);

        hashTable_insert(ht, key2, val2);
        index2 = hash_function(key2, ht->size);

        // Confirm that we have collision despites of using different keys
        assert(index == index2);
        assert(ht->records[index2] != NULL);

        // Two elements inserted despite of same indexes
        assert(ht->noOfElems == 2);

        // Previously written data should not be changed
        assert(strcmp(ht->records[index2]->key, key) == 0);
        assert(strcmp(ht->records[index2]->value, val) == 0);

        // But collision list should be now created
        assert(ht->collisionList[index2] != NULL);

        // New set of data should be added to collision list
        NodeList* firstNodeInCollision = ht->collisionList[index2];
        Record* rec2 = (Record*)firstNodeInCollision->data;
        assert(rec2 != NULL);
        assert(strcmp(rec2->key, key2) == 0);
        assert(strcmp(rec2->value, val2) == 0);
        assert(firstNodeInCollision->next == NULL);

        hashTable_insert(ht, key3, val3);
        index3 = hash_function(key3, ht->size);

        // Confirm that we have collision despites of using different keys
        assert(index2 == index3);
        assert(ht->records[index3] != NULL);

        // Three elements inserted despite of same indexes
        assert(ht->noOfElems == 3);

        // Firstly written data should not be changed
        assert(strcmp(ht->records[index3]->key, key) == 0);
        assert(strcmp(ht->records[index3]->value, val) == 0);

        // Same collision list should still exist
        assert(ht->collisionList[index2] == ht->collisionList[index3]);
        assert(ht->collisionList[index3] != NULL);

        // New set of data should be added to start of collision list
        NodeList* secondNodeInCollision = ht->collisionList[index3];
        Record* rec3 = (Record*)secondNodeInCollision->data;
        assert(rec3 != NULL);
        assert(strcmp(rec3->key, key3) == 0);
        assert(strcmp(rec3->value, val3) == 0);

        // Previous data should be accessable through new data
        assert(secondNodeInCollision->next != NULL);
        assert(secondNodeInCollision->next == firstNodeInCollision);
        Record* prevRec = (Record*)secondNodeInCollision->next->data;
        assert(rec2 == prevRec);
        assert(strcmp(prevRec->key, rec2->key) == 0);
        assert(strcmp(prevRec->value, rec2->value) == 0);
        assert(secondNodeInCollision->next->next == NULL);

        hashTable_delete(ht);
    }
}

void hashtable_search_test(void)
{
    {
        const size_t table_size = 10;
        const char* key = "keyOne";
        const char* val = "valOne";
        const char* key2 = "keyTwo";
        const char* val2 = "valTwo";
        HashTable* ht = hashTable_new(table_size);

        hashTable_insert(ht, key, val);
        hashTable_insert(ht, key2, val2);

        char* val_searched = hashTable_search(ht, key);
        char* val2_searched = hashTable_search(ht, key2);

        assert(val == val_searched);
        assert(val2 == val2_searched);
        assert(strcmp(val, val_searched) == 0);
        assert(strcmp(val2, val2_searched) == 0);

        hashTable_delete(ht);
    }

}