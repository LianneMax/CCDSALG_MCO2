#include "hash.h"

// Custom hash function adapted to use table_size as a parameter
unsigned int custom_hash(const char *input, unsigned int table_size) {
    unsigned int hash_value = 0;
    unsigned int prime_multiplier = 37;

    int i;
    for (i = 0; input[i] != '\0' && i < MAX_STRING_LENGTH; i++) {
        hash_value = (hash_value * prime_multiplier + input[i] * (i + 1)) & 0x7FFFFFFF;
    }
    return hash_value % table_size; // Use table_size to ensure hash fits
}

// Linear Probing for Collision Resolution
unsigned int resolve_collision(unsigned int index, unsigned int i, unsigned int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision
}

// Insert function
int insert(char *table[], unsigned int table_size, const char *str, unsigned int *collisions) {
    unsigned int index = custom_hash(str, table_size);
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
    unsigned int index = custom_hash(str, table_size);
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


