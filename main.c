#include "hash.h"

int main() {
    unsigned int n;
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

    fscanf(inputFile, "%u", &n);
    char key[MAX_STRING_LENGTH + 1];
    unsigned int stored = 0, inHomeAddress = 0, collisions = 0, comparisons = 0;

    while (fscanf(inputFile, "%s", key) == 1) {
        unsigned int index = hashFunction(key);
        unsigned int searchResult = search(key, hashTable, MAX_TABLE_SIZE);

        if (searchResult == -1) {
            insert(key, hashTable, MAX_TABLE_SIZE);
            stored++;
            if (hashFunction(key) == index) {
                inHomeAddress++;
            } else {
                collisions++;
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

    fprintf(outputFile, "%u\n%u\n%u\n%u\n%.6f\n", n, stored, inHomeAddress, collisions, (float)comparisons / stored);

    unsigned int i;
    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        if (hashTable[i]) {
            fprintf(outputFile, "%u %s %u %s %u\n", i, hashTable[i], hashFunction(hashTable[i]),
                    hashFunction(hashTable[i]) == i ? "YES" : "NO", 1);
        } else {
            fprintf(outputFile, "%u --- --- --- ---\n", i);
        }
    }
    fclose(outputFile);

    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        free(hashTable[i]);
    }
    free(hashTable);
    return EXIT_SUCCESS;
}