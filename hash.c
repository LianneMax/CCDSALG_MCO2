#include "hash.h"
#include <string.h>
#include <stdlib.h>

// Simplified Hash Function: djb2
int hash_function(const char *str, int table_size) {
    int hash = 5381; // Start with a large prime number
    while (*str) {
        hash = ((hash << 5) + hash) + *str; // hash * 33 + current character
        str++;
        if (hash < 0) {
            hash = -hash; // Ensure hash stays non-negative to avoid modulo issues
        }
    }
    return hash % table_size; // Fits within table size
}

// Linear Probing for Collision Resolution
int resolve_collision(int index, int i, int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision
}

// Insert function
int insert(char *table[], int table_size, const char *str, int *collisions) {
    int index = hash_function(str, table_size);
    int original_index = index;
    int i = 0;

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
    if (table[index] == NULL) {
        return -1; // Memory allocation failure
    }
    return index;
}

// Search function
int search(char *table[], int table_size, const char *str, int *comparisons) {
    int index = hash_function(str, table_size);
    int original_index = index;
    int i = 0;

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
void free_table(char *table[], int table_size) {
    int i;
    for (i = 0; i < table_size; i++) {
        if (table[i] != NULL) {
            free(table[i]);
            table[i] = NULL; // Set pointer to NULL after freeing
        }
    }
}