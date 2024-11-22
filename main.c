#include "hash.h"

// Function to find the next prime number greater than or equal to `num`
int next_prime(double num) {
    int int_part = (int)num; // Extract the integer part
    if (num > int_part) {    // Check if there is a fractional part
        int_part++;          // Increment to the next integer
    }

    int i; // Declare the loop variable outside the loop
    if (int_part <= 2) return 2; // Handle edge case for small numbers
    if (int_part % 2 == 0) int_part++; // Ensure odd starting number
    while (1) { // Loop until a prime is found
        int is_prime = 1; // Assume int_part is prime
        for (i = 2; i * i <= int_part; i++) { // Check divisibility
            if (int_part % i == 0) { // If divisible, it's not prime
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            return int_part; // Found the next prime
        }
        int_part += 2; // Skip even numbers
    }
}

int main(int argc, char *argv[]) {
    int n;
    int i; // Declare the loop variable at the top

    // Handle incorrect input usage
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

    if (fscanf(input, "%d", &n) != 1) { // Read the number of strings
        fprintf(stderr, "Error: Unable to read the number of strings from input file.\n");
        fclose(input);
        return EXIT_FAILURE;
    }

    // Compute hash table size
    double temp_size = 1.1 * n; // Calculate 1.1 * n
    int table_size = next_prime(temp_size); // Pass directly to next_prime

    // Debugging output
    printf("Computed hash table size: %d\n", table_size);

    char buffer[100000]; // Temporary storage for data from reading the input file
    size_t bytes_read = fread(buffer, sizeof(char), sizeof(buffer) - 1, input);
    fclose(input);

    if (bytes_read <= 0) {
        fprintf(stderr, "Error reading input file or file is empty.\n");
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0'; // Ensure null termination

    // Initialize hash table
    char *hash_table[table_size]; // Declare the hash table array
    for (i = 0; i < table_size; i++) {
        hash_table[i] = NULL;
    }

    int collisions = 0, total_comparisons = 0;
    int unique_strings = 0, stored_in_home = 0;

    // Tokenize input strings
    char *token = strtok(buffer, " \n");
    while (token != NULL) {

        //For debugging
        printf("Read string: %s\n", token);

        char str[MAX_STRING_LENGTH + 1];
        strncpy(str, token, MAX_STRING_LENGTH);
        str[MAX_STRING_LENGTH] = '\0'; // Ensure null termination

        int home_address = custom_hash(str, table_size);
        int result = insert(hash_table, table_size, str, &collisions);
        if (result >= 0) {
            unique_strings++;
            if (result == home_address) stored_in_home++;
        }

        token = strtok(NULL, " \n");
    }

    //Warning if unique_strings isnt the same with n
    if (unique_strings != n) {
    fprintf(stderr, "Warning: Expected %d strings but read %d.\n", n, unique_strings);
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
        for (i = 0; i < table_size; i++) {
            if (hash_table[i] != NULL) {
                int comparisons = 0;
                search(hash_table, table_size, hash_table[i], &comparisons);
                total_comparisons += comparisons;
            }
        }
        avg_comparisons = (double)total_comparisons / unique_strings;
    }

    // Write summary results
    fprintf(out, "%d\n", n);                            // Total strings read
    fprintf(out, "%d\n", unique_strings);               // Unique strings stored
    fprintf(out, "%d\n", stored_in_home);               // Stored in home addresses
    fprintf(out, "%d\n", unique_strings - stored_in_home); // Not stored in home
    fprintf(out, "%.6f\n\n", avg_comparisons);            // Average comparisons

    // Write hash table details
    for (i = 0; i < table_size; i++) {
        if (hash_table[i] != NULL) {
            int comparisons = 0;
            int home_address = custom_hash(hash_table[i], table_size);
            search(hash_table, table_size, hash_table[i], &comparisons);

            fprintf(out, "%-6d %-15s %-5d %-5s %-5d\n", i, hash_table[i], home_address,
                    (home_address == i ? "YES" : "NO"), comparisons);
        } else {
            fprintf(out, "%-6d %-15s %-5s %-5s %-5s\n", i, "---", "---", "---", "---");
        }
    }

    fclose(out);
    printf("Output written to %s\n", argv[2]);

    return EXIT_SUCCESS;
}
