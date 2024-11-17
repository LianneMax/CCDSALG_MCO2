#include "hash.h"

int main() {
    int n;
    char **hashTable = calloc(MAX_TABLE_SIZE, sizeof(char *));
    if (!hashTable) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    char inputFileName[256], outputFileName[256];
    printf("Enter input file name: ");
    scanf("%255s", inputFileName);

    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        perror("Failed to open input file");
        free(hashTable);
        return EXIT_FAILURE;
    }

    fscanf(inputFile, "%d", &n);
    char key[MAX_STRING_LENGTH + 1];
    int stored = 0, inHomeAddress = 0, collisions = 0, comparisons = 0;

    while (fscanf(inputFile, "%s", key) == 1) {
        int homeIndex = hashFunction(key);
        int searchResult = search(key, hashTable, MAX_TABLE_SIZE);

        if (searchResult == -1) {  // Key not found
            int finalIndex = homeIndex; // Store initial home address
            int attempt = 0;

            while (hashTable[finalIndex] != NULL) {  // Collision resolution
                attempt++;
                comparisons++;  // Count string comparisons during insert
                finalIndex = collisionResolution(homeIndex, attempt);
            }

            hashTable[finalIndex] = strdup(key);
            stored++;
            comparisons++;  // Add comparison for successful placement

            if (finalIndex == homeIndex) {
                inHomeAddress++;  // Count strings in their home address
            } else {
                collisions++;  // Count collisions
            }
        }
    }
    fclose(inputFile);

    printf("Enter output file name: ");
    scanf("%255s", outputFileName);

    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Failed to open output file");
        free(hashTable);
        return EXIT_FAILURE;
    }

    // Write summary to the output file
    fprintf(outputFile, "%d\n%d\n%d\n%d\n%.6f\n", n, stored, inHomeAddress, collisions, (float)comparisons / stored);

    // Write detailed hash table content
    int i;
    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        if (hashTable[i]) {
            int homeIndex = hashFunction(hashTable[i]);
            fprintf(outputFile, "%d %s %d %s %d\n", i, hashTable[i], homeIndex,
                    homeIndex == i ? "YES" : "NO", 1 + (i != homeIndex));  // Comparisons depend on collision
        } else {
            fprintf(outputFile, "%d --- --- --- ---\n", i);
        }
    }
    fclose(outputFile);

    // Clean up memory
    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        free(hashTable[i]);
    }
    free(hashTable);
    return EXIT_SUCCESS;
}