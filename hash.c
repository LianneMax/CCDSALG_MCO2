#include "hash.h"
#include <string.h>
#include <stdlib.h>

// Simplified Hash Function: djb2
int hash_function(const char *str, int table_size) {
    int hash = 5381; // Start with a large prime number
    while (*str) {
        hash = ((hash << 5) + hash) + *str; // hash * 33 + current character
        if (hash < 0) {
            hash = -hash; // Ensure hash stays non-negative to avoid modulo issues
        }
        str++;
    }
    return hash % table_size; // Fits within table size
}

// Linear Probing for Collision Resolution
int resolve_collision(int index, int i, int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision
}

// Handles collision resolution and finds the appropriate index for insertion
int collision_resolution(char *table[], int table_size, const char *str, int *collisions) {
    int home_address = hash_function(str, table_size);
    int index = home_address;
    int i = 0;

    while (table[index] != NULL) {
        if (strcmp(table[index], str) == 0) {
            // String already exists; no insertion
            printf("Duplicate detected for string: %s at index %d\n", str, index); // Debugging
            return -1;
        }

        // Increment collisions for unique strings encountering an occupied slot
        (*collisions)++;
        printf("Collision detected for string: %s at index %d\n", str, index); // Debugging
        printf("The value of collision after detection is %n", collisions);

        i++;
        index = resolve_collision(home_address, i, table_size);
        printf("Probing to index %d for string: %s\n", index, str); // Debugging
    }

    // Return the resolved index for insertion
    return index;
}

int insert(char *table[], int table_size, const char *str, int *collisions, int *stored_in_home) {
    // Call collision_resolution to handle probing and collision detection
    int index = collision_resolution(table, table_size, str, collisions);
    if (index == -1) {
        printf("Duplicate detected for string: %s\n", str); // Debugging
        return -1; // String already exists
    }

    // Insert the string into the hash table
    table[index] = strdup(str);
    if (!table[index]) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Check if stored at home address
    int home_address = hash_function(str, table_size);
    if (index == home_address) {
        (*stored_in_home)++;
        printf("String %s stored at home address %d\n", str, home_address); // Debugging
    } else {
        printf("String %s stored at index %d after resolving collision\n", str, index); // Debugging
    }
    printf("------------------------------------------------\n"); //delete when the program works

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