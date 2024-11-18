#include "hash.h"

// Simplified Hash Function: djb2
unsigned int hash_function(const char *str, unsigned int table_size) {
    unsigned int hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + *str; // hash * 33 + current character
        str++;



    }
    return hash % table_size; // Ensure it fits within the table size
}

// Linear Probing for Collision Resolution
unsigned int resolve_collision(unsigned int index, unsigned int i, unsigned int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision
}

// Insert function
int insert(char *table[], unsigned int table_size, const char *str, unsigned int *collisions) {
    unsigned int index = hash_function(str, table_size);
    unsigned int original_index = index;
    unsigned int i = 0;

    while (table[index] != NULL) {
        if (strcmp(table[index], str) == 0) {
            return -1; // String already exists
        }
        (*collisions)++;
        i++;
        index = resolve_collision(original_index, i, table_size);
        if (index == original_index) {
            return -1; // Table is full
        }


    }
    table[index] = strdup(str); // Store the string
    return index;
}

// Search function
int search(char *table[], unsigned int table_size, const char *str, unsigned int *comparisons) {
    unsigned int index = hash_function(str, table_size);
    unsigned int original_index = index;
    unsigned int i = 0;

    while (table[index] != NULL) {
        (*comparisons)++;
        if (strcmp(table[index], str) == 0) {
            return index; // String found
        }
        i++;
        index = resolve_collision(original_index, i, table_size);
        if (index == original_index) {
            break; // Full loop, string not found

        }
    }
    return -1; // Not found
}

// Free hash table memory
void free_table(char *table[], unsigned int table_size) {
    unsigned int i; // Declare the loop variable outside
    for (i = 0; i < table_size; i++) {
        if (table[i] != NULL) {
            free(table[i]);
    }
}
}