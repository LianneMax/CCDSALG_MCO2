#include "hash.h"

/*  Custom Hash Function
    - djb2 and FNV hash were used for inspiration
    - uses a prime multiplier (prime_multiplier)
 */
 int custom_hash(const char *input,  int table_size) {
     int hash_value = 0; //will hold the computed hash during the loop
     int prime_multiplier = 37;

    int i;
    /*  the loop will continue until it reaches the end of the string OR
        the MAX_STRING_LENGTH is met
    */ 
    for (i = 0; input[i] != '\0' && i < MAX_STRING_LENGTH; i++) {
        /*  A. Each character's ASCII valued is mmultiplied by its position in the string
                - To introduce positional information into the hash

            B. The current hash_value is multiplied by the prime_multiplier (37)
                - Spreads the bits to make sure that small differences in 
        */
        hash_value = (hash_value * prime_multiplier + input[i] * (i + 1)) & 0x7FFFFFFF;
    }
    return hash_value % table_size; // Use table_size to ensure hash fits
}

// Linear Probing for Collision Resolution
  int resolve_collision( int index,  int i,  int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision
}

// Insert function
int insert(char *table[],  int table_size, const char *str,  int *collisions) {
     int index = custom_hash(str, table_size);
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
    return index;
}

// Search function
int search(char *table[],  int table_size, const char *str,  int *comparisons) {
     int index = custom_hash(str, table_size);
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


