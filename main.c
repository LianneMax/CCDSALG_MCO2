#include "hash.h"

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);


        return EXIT_FAILURE;
    }

    // Open input file
    FILE *in = fopen(argv[1], "r");
    if (!in) {
        perror("Error opening input file");
        return EXIT_FAILURE;
    }

    unsigned int n;
    fscanf(in, "%u", &n); // Read the number of strings

    char buffer[100000];
    size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer) - 1, in);
    fclose(in);

    if (bytes_read <= 0) {
        fprintf(stderr, "Error reading input file or file is empty.\n");
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0'; // Ensure null termination

    // Initialize hash table
    char *hash_table[HASH_TABLE_SIZE] = {NULL};
    unsigned int collisions = 0;
    unsigned int total_comparisons = 0;

    // Tokenize input strings
    char *token = strtok(buffer, " \n");
    unsigned int unique_strings = 0, stored_in_home = 0;

    while (token != NULL) {
        char str[MAX_STRING_LENGTH + 1];
        strncpy(str, token, MAX_STRING_LENGTH);
        str[MAX_STRING_LENGTH] = '\0'; // Ensure null termination

        unsigned int home_address = hash_function(str, HASH_TABLE_SIZE);
        int result = insert(hash_table, HASH_TABLE_SIZE, str, &collisions);
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
        free_table(hash_table, HASH_TABLE_SIZE);
        return EXIT_FAILURE;
    }

    // Write summary results
    fprintf(out, "%u\n", n);
    fprintf(out, "%u\n", unique_strings);
    fprintf(out, "%u\n", stored_in_home);
    fprintf(out, "%u\n", collisions);
    fprintf(out, "%.6f\n", (unique_strings > 0) ? (double)total_comparisons / unique_strings : 0);

    // Write hash table details
    unsigned int i; // Declare loop variable outside the loop
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            unsigned int comparisons = 0;
            unsigned int home_address = hash_function(hash_table[i], HASH_TABLE_SIZE);
            search(hash_table, HASH_TABLE_SIZE, hash_table[i], &comparisons);
            fprintf(out, "%u %s %u %s %u\n", i, hash_table[i], home_address,
                    (home_address == i) ? "YES" : "NO", comparisons);
            total_comparisons += comparisons;
        } else {
            fprintf(out, "%u --- --- --- ---\n", i);
        }
    }


    fclose(out);
    free_table(hash_table, HASH_TABLE_SIZE);
    printf("Output written to %s\n", argv[2]);


    return EXIT_SUCCESS;
}