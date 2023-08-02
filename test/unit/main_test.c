// Hashtable tests
extern void hashtable_record_new_test(void);
extern void hashtable_new_test(void);
extern void hashtable_insert_test(void);
extern void hashtable_delete_record_test(void);
extern void hashtable_hash_function_test(void);
extern void hashtable_collision_test(void);
extern void hashtable_search_test(void);

// Linked list tests
extern void nodeList_new_test(void);
extern void nodeList_insert_test(void);
extern void nodeList_node_delete_test(void);

int main(void)
{
    hashtable_record_new_test();
    hashtable_new_test();
    hashtable_insert_test();
    hashtable_delete_record_test();
    hashtable_hash_function_test();
    hashtable_collision_test();
    hashtable_search_test();

    nodeList_new_test();
    nodeList_insert_test();
    nodeList_node_delete_test();

    return 0;
}