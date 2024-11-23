#include "hash.h"

/* Custom Hash Function
   - Uses modular arithmetic to avoid overflow and ensure positive values.
   - Incorporates character values and positional information.
   - A smaller prime multiplier (31) ensures good distribution for typical text inputs.
*/
unsigned int custom_hash(const char *input, int table_size) {
    unsigned int hash_value = 0;           // Holds the computed hash during the loop.
    int prime_multiplier = 37;   // Prime multiplier for better distribution.
    int i;

    for (i = 0; input[i] != '\0' && i < MAX_STRING_LENGTH; i++) {
        /*
        1. Multiply the current hash value by the prime multiplier (37).
        2. Add the ASCII value of the current character adjusted by (i + 1) for positional influence.
        3. Use modular arithmetic to keep the value within a consistent range.
        */
        hash_value = (hash_value * prime_multiplier + input[i] *(i + 1));
    }

    // Ensure the hash value fits within the table size.
    return hash_value % table_size;
}

// Linear Probing for Collision Resolution
int resolve_collision(int index, int i, int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision.
}

// Insert Function
int insert(char *table[], int table_size, const char *str, int *collisions) {
    int index = custom_hash(str, table_size); // Compute the initial hash (home address).
    int original_index = index;              // Save the original index for circular probing.
    int i = 0;

    // Probe until an empty slot is found or the table is full.
    while (table[index] != NULL) {
        if (strcmp(table[index], str) == 0) {
            return -1; // String already exists.
        }
        (*collisions)++; // Increment collision count.
        i++;
        index = resolve_collision(original_index, i, table_size); // Probe to the next index.

        // If we have looped back to the original index, the table is full.
        if (index == original_index) {
            return -1;
        }
    }

    // Store the string in the empty slot and return the index.
    table[index] = strdup(str);
    return index;
}

// Search Function
int search(char *table[], int table_size, const char *str, int *comparisons) {
    int index = custom_hash(str, table_size); // Compute the initial hash (home address).
    int original_index = index;              // Save the original index for circular probing.
    int i = 0;

    // Probe until the string is found or we loop back to the original index.
    while (table[index] != NULL) {
        (*comparisons)++; // Increment comparison count.

        // If the string matches the current slot, return the index.
        if (strcmp(table[index], str) == 0) {
            return index;
        }

        i++;
        index = resolve_collision(original_index, i, table_size); // Probe to the next index.

        // If we have looped back to the original index, the string is not in the table.
        if (index == original_index) {
            break;
        }
    }

    return -1; // String not found.
}
