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

    int n;
    fscanf(in, "%d", &n); // Read the number of strings

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
    int collisions = 0;
    int total_comparisons = 0;
    int unique_strings = 0, stored_in_home = 0;

    // Tokenize input strings
    char *token = strtok(buffer, " \n");
    int temp = 1; // For debugging purposes

    while (token != NULL) {
        char str[MAX_STRING_LENGTH + 1];
        strncpy(str, token, MAX_STRING_LENGTH);
        str[MAX_STRING_LENGTH] = '\0'; // Ensure null termination

        //This should be invoked once per string
        int home_address = hash_function(str, HASH_TABLE_SIZE);
        printf("The function hash_function ran for %d times\n", temp); //delete once the program is working

        //This should be invoked once per string
        int result = insert(hash_table, HASH_TABLE_SIZE, str, &collisions, &stored_in_home);
        printf("Collisions after inserting %s: %d\n", str, collisions); // delete once the program is working
        printf("The insert function ran for %d times\n", temp); //delete once the program is working
        printf("the value of collisions is %d\n", collisions); // Debugging statement

        if (result >= 0) {
            unique_strings++;
            printf("unique strings is %d\n", unique_strings); // Debugging statement

            if (result == home_address) {
                stored_in_home++;
                printf("stored in home is %d\n", stored_in_home); // Debugging statement
            }
        }

        printf("--------------------------------------\n\n\n");
        token = strtok(NULL, " \n");
        temp++; // Increment debug counter
    }

    // Open output file
    FILE *out = fopen(argv[2], "w");
    if (!out) {
        perror("Error opening output file");
        free_table(hash_table, HASH_TABLE_SIZE);
        return EXIT_FAILURE;
    }

    // Write summary results
    fprintf(out, "%d\n", n); //Line1
    fprintf(out, "%d\n", unique_strings); //Line 2
    fprintf(out, "%d\n", stored_in_home); //Line 3
    fprintf(out, "%d\n", collisions); //Line 4
    fprintf(out, "%.6f\n", (unique_strings > 0) ? (double)total_comparisons / unique_strings : 0);

    // Write hash table details
    int i;
    for (i = 0; i < HASH_TABLE_SIZE; i++) {
        if (hash_table[i] != NULL) {
            int comparisons = 0;
            int home_address = hash_function(hash_table[i], HASH_TABLE_SIZE);
            search(hash_table, HASH_TABLE_SIZE, hash_table[i], &comparisons);
            fprintf(out, "%d %s %d %s %d\n", i, hash_table[i], home_address,
                    (home_address == i) ? "YES" : "NO", comparisons);
            total_comparisons += comparisons;
        } else {
            fprintf(out, "%d --- --- --- ---\n", i);
        }
    }

    fclose(out);
    free_table(hash_table, HASH_TABLE_SIZE);
    printf("Output written to %s\n", argv[2]);

    return EXIT_SUCCESS;
}
