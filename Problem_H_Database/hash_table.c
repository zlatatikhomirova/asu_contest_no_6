#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 50000 // hash table capacity

unsigned long hash_function(char* str) {
    unsigned long i = 0;
    for (int j = 0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

typedef struct Ht_item Ht_item;

// Define the Hash Table Item here
struct Ht_item {
    char* key;
    char* value;
};


typedef struct LinkedList LinkedList;

// Define the Linkedlist here
struct LinkedList {
    Ht_item* item;
    LinkedList* next;
};


typedef struct HashTable HashTable;

// Define the Hash Table here
struct HashTable {
    // Contains an array of pointers
    // to items
    Ht_item** items;
    LinkedList** overflow_buckets;
    int size;
    int count;
};


static LinkedList* allocate_list() {
    // Allocates memory for a Linkedlist pointer
    LinkedList* list = (LinkedList*)calloc(1, sizeof(LinkedList));
    return list;
}

static LinkedList* linkedlist_insert(LinkedList* list, Ht_item* item) {
    // Inserts the item onto the Linked List
    if (!list) {
        LinkedList* head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }

    else {
        LinkedList* temp = list;
        while (temp->next) {
            temp = temp->next;
        }

        LinkedList* node = allocate_list();
        node->item = item;
        node->next = NULL;
        temp->next = node;

        return list;
    }
}

static void free_linkedlist(LinkedList* list) {
    LinkedList* temp = list;
    if (!list)
        return;
    while (list) {
        temp = list;
        list = list->next;
        free(temp->item->key);
        free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

static LinkedList** create_overflow_buckets(HashTable* table) {
    // Create the overflow buckets; an array of linkedlists
    LinkedList** buckets = (LinkedList**)calloc(table->size, sizeof(LinkedList*));
    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;
    return buckets;
}

static void free_overflow_buckets(HashTable* table) {
    // Free all the overflow bucket lists
    LinkedList** buckets = table->overflow_buckets;
    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);
    free(buckets);
}


Ht_item* create_item(char* key, char* value) {
    // Creates a pointer to a new hash table item
    Ht_item* item = (Ht_item*)malloc(sizeof(Ht_item));
    item->key = (char*)calloc(strlen(key) + 1, sizeof(char));
    item->value = (char*)calloc(strlen(value) + 1, sizeof(char));

    strcpy(item->key, key);
    strcpy(item->value, value);

    return item;
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item**)calloc(table->size, sizeof(Ht_item*));
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;
    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item* item) {
    // Frees an item
    free(item->key);
    free(item->value);
    free(item);
}

void free_hashtable(HashTable* table) {
    // Frees the table
    for (int i = 0; i < table->size; i++) {
        Ht_item* item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable* table, unsigned long index, Ht_item* item) {
    LinkedList* head = table->overflow_buckets[index];

    if (head == NULL) {
        // We need to create the list
        head = allocate_list();
        head->item = item;
        head->next = NULL;
        table->overflow_buckets[index] = head;
        return;
    }
    else {
        // Consider the case: when inserting two nodes with same key into the overflow_buckets
        LinkedList* curr = table->overflow_buckets[index];
        while (curr) {
            if (strcmp(curr->item->key, item->key) == 0) {
                free_item(curr->item);
                curr->item = item;
                return;
            }
            curr = curr->next;
        }

        // Insert to the list
        table->overflow_buckets[index] = linkedlist_insert(head, item);
        return;
    }
}

void ht_insert(HashTable* table, char* key, char* value) {
    // Create the item
    Ht_item* item = create_item(key, value);

    // Compute the index
    int index = hash_function(key);

    Ht_item* current_item = table->items[index];

    if (current_item == NULL) {
        // Key does not exist.
        if (table->count == table->size) {
            // Hash Table Full
            printf("Insert Error: Hash Table is full\n");
            // Remove the create item
            free_item(item);
            return;
        }

        // Insert directly
        table->items[index] = item;
        table->count++;
    }

    else {
        // Scenario 1: We only need to update value for the same key
        if (strcmp(current_item->key, key) == 0) {
            free(table->items[index]->value);
            table->items[index]->value = (char*)calloc(strlen(value) + 1, sizeof(char));
            strcpy(table->items[index]->value, value);
            free_item(item);
            return;
        }

        else {
            // Scenario 2: Collision
            handle_collision(table, index, item);
            return;
        }
    }
}

char* ht_search(HashTable* table, char* key) {
    // Searches the key in the hashtable
    // and returns NULL if it doesn't exist
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    // Ensure that we move to items which are not NULL
    while (item != NULL) {
        if (strcmp(item->key, key) == 0)
            return item->value;
        if (head == NULL)
            return NULL;
        item = head->item;
        head = head->next;
    }
    return NULL;
}

void ht_delete(HashTable* table, char* key) {
    // Deletes an item from the table
    int index = hash_function(key);
    Ht_item* item = table->items[index];
    LinkedList* head = table->overflow_buckets[index];

    if (item == NULL) {
        // Does not exist. Return
        return;
    }
    else {
        if (head == NULL && strcmp(item->key, key) == 0) {
            // No collision chain. Remove the item
            // and set table index to NULL
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL) {
            // Collision Chain exists
            if (strcmp(item->key, key) == 0) {
                // Remove this item and set the head of the list
                // as the new item

                free_item(item);
                LinkedList* node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList* curr = head;
            LinkedList* prev = NULL;

            while (curr) {
                if (strcmp(curr->item->key, key) == 0) {
                    if (prev == NULL) {
                        // First element of the chain. Remove the chain                 
                        table->overflow_buckets[index] = head->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        return;
                    }
                    else {
                        // This is somewhere in the chain
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        return;
                    }
                }

                prev = curr;
                curr = curr->next;
            }

        }
    }

}
