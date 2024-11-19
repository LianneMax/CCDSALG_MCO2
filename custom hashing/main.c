#include "hash.h"

// Function to find the next prime number greater than `num`
unsigned int next_prime(unsigned int num) {
    if (num <= 2) return 2; // Special case for small numbers
    num++; // Start searching from the next number
    unsigned int i; // Declare loop variable outside
    while (1) {
        int is_prime = 1;
        for (i = 2; i * i <= num; i++) { // Check divisibility
            if (num % i == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) return num; // Found the next prime
        num++;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open input file
    FILE *input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    unsigned int n;
    if (fscanf(input, "%u", &n) != 1) { // Read the number of strings
        fprintf(stderr, "Error: Unable to read the number of strings from input file.\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    // Compute hash table size
    unsigned int table_size = next_prime((unsigned int)(1.1 * n));
    printf("Computed hash table size: %u\n", table_size);

    char buffer[100000];
    size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer) - 1, input);
    fclose(input);

    if (bytes_read <= 0) {
        fprintf(stderr, "Error reading input file or file is empty.\n");
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0'; // Ensure null termination

    // Initialize hash table
    char **hash_table = calloc(table_size, sizeof(char *));
    if (!hash_table) {
        fprintf(stderr, "Error allocating memory for hash table.\n");
        return EXIT_FAILURE;
    }

    unsigned int collisions = 0, total_comparisons = 0;
    unsigned int unique_strings = 0, stored_in_home = 0;
    unsigned int i; // Declare loop variable outside

    // Tokenize input strings
    char *token = strtok(buffer, " \n");
    while (token != NULL) {
        char str[MAX_STRING_LENGTH + 1];
        strncpy(str, token, MAX_STRING_LENGTH);
        str[MAX_STRING_LENGTH] = '\0'; // Ensure null termination

        unsigned int home_address = custom_hash(str, table_size);
        int result = insert(hash_table, table_size, str, &collisions);
        if (result >= 0) {
            unique_strings++;
            if (result == (int)home_address) stored_in_home++;
        }

        token = strtok(NULL, " \n");
    }

    // Open output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        perror("Error opening output file");
        return EXIT_FAILURE;
    }

    // Calculate average comparisons
    double avg_comparisons = 0;
    if (unique_strings > 0) {
        for (i = 0; i < table_size; i++) { // Use externally declared variable
            if (hash_table[i] != NULL) {
                unsigned int comparisons = 0;
                search(hash_table, table_size, hash_table[i], &comparisons);
                total_comparisons += comparisons;
            }
        }
        avg_comparisons = (double)total_comparisons / unique_strings;
    }

    // Write summary results
    fprintf(out, "%u\n", n);                            // Total strings read
    fprintf(out, "%u\n", unique_strings);               // Unique strings stored
    fprintf(out, "%u\n", stored_in_home);               // Stored in home addresses
    fprintf(out, "%u\n", unique_strings - stored_in_home); // Not stored in home
    fprintf(out, "%.6f\n\n", avg_comparisons);            // Average comparisons

    // Write hash table details
    for (i = 0; i < table_size; i++) { // Use externally declared variable
        if (hash_table[i] != NULL) {
            unsigned int comparisons = 0;
            unsigned int home_address = custom_hash(hash_table[i], table_size);
            search(hash_table, table_size, hash_table[i], &comparisons);

            char *home_match = "NO"; // Default to NO
            if (home_address == i) {
                home_match = "YES"; // Update to YES if stored in the home address
            }

            fprintf(out, "%-6u %-15s %-5u %-5s %-5u\n", i, hash_table[i], home_address, home_match, comparisons);
        } else {
            fprintf(out, "%-6u %-15s %-5s %-5s %-5s\n", i, "---", "---", "---", "---");
        }
    }


    fclose(out);
    printf("Output written to %s\n", argv[2]);

    return EXIT_SUCCESS;
}





















