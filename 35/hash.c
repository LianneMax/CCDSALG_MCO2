#include "hash.h"

/**
 * @brief Custom hash function to compute the hash value for a given string.
 * 
 * @details
 * - Uses modular arithmetic to avoid overflow and ensure non-negative values.
 * - Incorporates character values and positional information for better distribution.
 * - A prime multiplier (37) is used to reduce the risk of collisions for typical text inputs.
 * - Returns an unsigned integer to:
 *   - Ensure non-negative indices.
 *   - Provide a larger value range for reducing collision risk.
 *   - Avoid complications with negative values during modulo operations.
 * 
 * @param input A null-terminated string to be hashed.
 * @param table_size The size of the hash table.
 * @return unsigned int The computed hash value modulo the table size.
 */

unsigned int custom_hash(const char *input, int table_size) {
    unsigned int hash_value = 0;  // Holds the computed hash during the loop.
    int prime_multiplier = 37;   // Prime multiplier for better distribution.
    int i;

    for (i = 0; input[i] != '\0' && i < MAX_STRING_LENGTH; i++) {
        /*
        1. Multiply the current hash value by the prime multiplier (37).
        2. Add the ASCII value of the current character adjusted by (i + 1) for positional influence.
        */
        hash_value = (hash_value * prime_multiplier + input[i] *(i + 1));
    }

    // Ensure the hash value fits within the table size
    // also returns the unsigned integer after modulo operation with the table_size
    return hash_value % table_size;
}

/**
 * @brief Resolves collisions using linear probing.
 * 
 * @details
 * - Increments the index by 1 (or by the current iteration count) on each collision.
 * - Ensures the index wraps around using modulo arithmetic.
 * 
 * @param index The initial index (home address) from the hash function.
 * @param i The current iteration count in the probing sequence.
 * @param table_size The size of the hash table.
 * @return int The new index after applying linear probing.
 */

int resolve_collision(int index, int i, int table_size) {
    return (index + i) % table_size; // Increment index by 1 (i) on each collision.
}

/**
 * @brief Inserts a string into the hash table, handling collisions using linear probing.
 * 
 * @details
 * - Computes the home address using the custom hash function.
 * - Resolves collisions using linear probing.
 * - Avoids duplicate entries by checking if the string already exists in the table.
 * 
 * @param table The hash table (array of strings).
 * @param table_size The size of the hash table.
 * @param str The string to be inserted.
 * @param collisions Pointer to an integer tracking the number of collisions.
 * @return int The index where the string was inserted, or -1 if it already exists or the table is full.
 */

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

/**
 * @brief Searches for a string in the hash table.
 * 
 * @details
 * - Computes the home address using the custom hash function.
 * - Probes for the string using linear probing, tracking the number of comparisons.
 * - Ends the search early if the string is found or the probing sequence loops back.
 * 
 * @param table The hash table (array of strings).
 * @param table_size The size of the hash table.
 * @param str The string to search for.
 * @param comparisons Pointer to an integer tracking the number of comparisons made.
 */

void search(char *table[], int table_size, const char *str, int *comparisons) {
    int index = custom_hash(str, table_size); // Compute the initial hash (home address).
    int original_index = index;              // Save the original index for circular probing.
    int i = 0;

    // Probe until the string is found or we loop back to the original index.
    while (table[index] != NULL) {
        (*comparisons)++; // Increment comparison count.

        // If the string matches the current slot, return the index.
        if (strcmp(table[index], str) == 0) {
            break;
        }

        i++;
        index = resolve_collision(original_index, i, table_size); // Probe to the next index.

        // If we have looped back to the original index, the string is not in the table.
        if (index == original_index) {
            //ends the loop early
            break;
        }
    }
}
