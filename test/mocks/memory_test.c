#include <stdlib.h> // malloc, calloc
#include <assert.h>
#include <stdbool.h>

void hashtable_record_new_test(void);
void hashtable_new_test(void);
void nodeList_new_test(void);

static struct
{
    bool malloc_null;
    bool calloc_null;
} mock_params;

static inline void* mock_malloc(size_t size)
{
    if (mock_params.malloc_null == false)
    {
        return malloc(size);
    }

    return NULL;
}

static inline void* mock_calloc(size_t nmemb, size_t size)
{
    if (mock_params.calloc_null == false)
    {
        return calloc(nmemb, size);
    }
    
    return NULL;
}

#define malloc(size) mock_malloc(size)
#define calloc(nmemb, size) mock_calloc(nmemb, size)

#include <hashtable.c>
#include <nodelist.c>


// Test function: Record* record_new(const char* key, const char* value);
void hashtable_record_new_test(void)
{
    // Both arguments correct, check behaviour when malloc fails
    {
        mock_params.malloc_null = true;
        mock_params.calloc_null = false;
        const char* key = "exampKey";
        const char* val = "someValue";
        Record* record = record_new(key, val);

        assert(record == NULL);
    }
}


// Test function: HashTable* hashTable_new(size_t size);
void hashtable_new_test(void)
{
    // Correct argument, check behaviour when malloc fails
    {
        mock_params.malloc_null = true;
        mock_params.calloc_null = false;
        const size_t table_size = 5;
        HashTable* hashTable = hashTable_new(table_size);

        assert(hashTable == NULL);
    }

    // Correct argument, check behaviour when calloc fails
    {
        mock_params.malloc_null = false;
        mock_params.calloc_null = true;
        const size_t table_size = 5;
        HashTable* hashTable = hashTable_new(table_size);

        assert(hashTable == NULL);
    }
}

// Test function: NodeList* nodeList_new(void* data); 
void nodeList_new_test(void)
{
    // Pass proper argument to constructor, check behaviour when malloc fails
    {
        mock_params.malloc_null = true;
        mock_params.calloc_null = false;

        Record* record = record_new("key1", "val1");
        NodeList* node = nodeList_new(record);

        assert(node == NULL);
        
        record_delete(record);
    }
}