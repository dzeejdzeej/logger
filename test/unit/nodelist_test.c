#include <hashtable.c>
#include <nodelist.c>
#include <assert.h>
#include <stdio.h>

void nodeList_new_test(void);
void nodeList_insert_test(void);
void nodeList_node_delete_test(void);


// Test function: NodeList* nodeList_new(void* data);
void nodeList_new_test(void)
{
    // pass NULL pointer as an argument to constructor
    {
        Record* record = NULL;
        NodeList* node = nodeList_new(record);

        assert(node == NULL);
    }

    // pass proper argument to constructor
    {
        Record* record = record_new("key1", "val1");
        NodeList* node = nodeList_new(record);

        assert(node != NULL);
        assert(node->next == NULL);
        assert(node->data == record);
        
        nodeList_delete(&node);
        record_delete(record);
    }
}

// Test function: void nodeList_insert(NodeList** head, void* data);
void nodeList_insert_test(void)
{
    // incorrect insert, NULL head as an argument, head should remain NULL
    {
        Record* record1 = record_new("key1", "val1");
        NodeList* head = NULL;
        nodeList_insert(&head, record1);

        assert(head == NULL);

        record_delete(record1);
    }

    // incorrect insert, proper head but NULL record, head should not be changed
    {
        Record* record1 = record_new("key1", "val1");
        Record* record2 = NULL;
        NodeList* head = nodeList_new(record1);
        NodeList* head_cpy = head;
        nodeList_insert(&head, record2);

        assert(head == head_cpy);
        
        nodeList_delete(&head);
        record_delete(record1);
    }

    // incorrect insert, both arguments NULL, head should remain NULL
    {
        Record* record = NULL;
        NodeList* head = NULL;
        nodeList_insert(&head, record);

        assert(head == NULL);
    }

    // proper insert, check if inserts works well
    {
        Record* record1 = record_new("key1", "val1");
        Record* record2 = record_new("key2", "val2");
        Record* record3 = record_new("key3", "val3");

        NodeList* head = nodeList_new(record1);
        assert(head != NULL);
        assert(head->next == NULL);
        assert(head->data == record1);

        NodeList* head_temp = head;
        nodeList_insert(&head, record2);
        assert(head != NULL);
        assert(head->next == head_temp);
        assert(head->data == record2);

        head_temp = head;
        nodeList_insert(&head, record3);
        assert(head != NULL);
        assert(head->next == head_temp);
        assert(head->data == record3);

        nodeList_delete(&head);
        record_delete(record1);
        record_delete(record2);
        record_delete(record3);
    }
}

// Test function: void nodeList_node_delete(NodeList** head, void* data)
void nodeList_node_delete_test(void)
{
    // Delete firstly added node out of three
    {
        Record* record1 = record_new("key1", "val1");
        Record* record2 = record_new("key2", "val2");
        Record* record3 = record_new("key3", "val3");

        NodeList* head = nodeList_new(record1);
        NodeList* node1 = head;

        nodeList_insert(&head, record2);
        NodeList* node2 = head;

        nodeList_insert(&head, record3);
        NodeList* node3 = head;

        assert(head == node3);
        assert(node3->next == node2);
        assert(node2->next == node1);
        assert(node1->next == NULL);

        nodeList_node_delete(&head, record1);

        assert(head == node3);
        assert(node3->next == node2);
        assert(node2->next == NULL);

        nodeList_delete(&head);
        record_delete(record1);
        record_delete(record2);
        record_delete(record3);
    }

    // Delete secondly added node out of three
    {
        Record* record1 = record_new("key1", "val1");
        Record* record2 = record_new("key2", "val2");
        Record* record3 = record_new("key3", "val3");

        NodeList* head = nodeList_new(record1);
        NodeList* node1 = head;

        nodeList_insert(&head, record2);
        NodeList* node2 = head;

        nodeList_insert(&head, record3);
        NodeList* node3 = head;

        assert(head == node3);
        assert(node3->next == node2);
        assert(node2->next == node1);
        assert(node1->next == NULL);

        nodeList_node_delete(&head, record2);

        assert(head == node3);
        assert(node3->next == node1);
        assert(node1->next == NULL);

        nodeList_delete(&head);
        record_delete(record1);
        record_delete(record2);
        record_delete(record3);
    }

    // Delete last added node (actually a head) out of three
    {
        Record* record1 = record_new("key1", "val1");
        Record* record2 = record_new("key2", "val2");
        Record* record3 = record_new("key3", "val3");

        NodeList* head = nodeList_new(record1);
        NodeList* node1 = head;

        nodeList_insert(&head, record2);
        NodeList* node2 = head;

        nodeList_insert(&head, record3);
        NodeList* node3 = head;

        assert(head == node3);
        assert(node3->next == node2);
        assert(node2->next == node1);
        assert(node1->next == NULL);

        nodeList_node_delete(&head, record3);
        record_delete(record3);

        assert(head == node2);
        assert(node2->next == node1);
        assert(node1->next == NULL);

        nodeList_delete(&head);
        record_delete(record1);
        record_delete(record2);
    }

}
