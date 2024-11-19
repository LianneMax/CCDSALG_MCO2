#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STR_LEN 15           // Maximum string length (excluding null byte)
#define PRIME_MODULUS 18031      // First prime > 1.1 * 16384
#define HASH_TABLE_SIZE 18031    // Hash table size (same as PRIME_MODULUS)

// Hash table to store unique strings
char *hash_table[HASH_TABLE_SIZE] = {NULL};

// Custom hash function
uint32_t custom_hash(const char *input) {
    uint32_t hash_value = 0;
    uint32_t prime_multiplier = 37;

    int i;
    for (i = 0; input[i] != '\0' && i < MAX_STR_LEN; i++) {
        hash_value = (hash_value * prime_multiplier + input[i] * (i + 1)) & 0x7FFFFFFF;
    }
    return hash_value % PRIME_MODULUS;
}

// Collision resolution using linear probing
int resolve_collision(const char *key) {
    uint32_t index = custom_hash(key);

    // Linear probing to find an empty slot
    while (hash_table[index] != NULL) {
        if (strcmp(hash_table[index], key) == 0) {
            // Key already exists in hash table
            return -1;
        }
        index = (index + 1) % HASH_TABLE_SIZE;
    }
    return index;
}

// Function to insert a string into the hash table
int insert_into_hash_table(const char *key) {
    int index = custom_hash(key);

    if (hash_table[index] == NULL) {
        hash_table[index] = strdup(key);  // Store key at home address
        return 0;
    } else {
        int resolved_index = resolve_collision(key);
        if (resolved_index != -1) {
            hash_table[resolved_index] = strdup(key);
            return 1;
        }
    }
    return -1;
}

// Main program
int main() {
    char input_filename[256], output_filename[256];
    FILE *input_file, *output_file;
    char buffer[MAX_STR_LEN + 1];
    int total_strings = 0, unique_strings = 0, collisions = 0;

    // Prompt for input file
    printf("Enter the name of the input file: ");
    scanf("%255s", input_filename);

    // Open input file
    input_file = fopen(input_filename, "r");
    if (input_file == NULL) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    // Read strings from the input file
    while (fscanf(input_file, "%15s", buffer) != EOF) {
        total_strings++;
        int result = insert_into_hash_table(buffer);
        if (result == 0) {
            unique_strings++;
        } else if (result == 1) {
            collisions++;
        }
    }
    fclose(input_file);

    // Prompt for output file
    printf("Enter the name of the output file: ");
    scanf("%255s", output_filename);

    // Open output file
    output_file = fopen(output_filename, "w");
    if (output_file == NULL) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    // Write results to the output file
    fprintf(output_file, "%d\n", total_strings);
    fprintf(output_file, "%d\n", unique_strings);
    fprintf(output_file, "%d\n", unique_strings - collisions);
    fprintf(output_file, "%d\n", collisions);

    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            fprintf(output_file, "%d %s %d YES 1\n", i, hash_table[i], i);
        } else {
            fprintf(output_file, "%d --- --- --- ---\n", i);
        }
    }

    fclose(output_file);

    // Free allocated memory
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            free(hash_table[i]);
        }
    }

    printf("Processing complete. Results written to %s\n", output_filename);
    return EXIT_SUCCESS;
}