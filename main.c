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
        int index = hashFunction(key);
        int searchResult = search(key, hashTable, MAX_TABLE_SIZE);

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

    fprintf(outputFile, "%d\n%d\n%d\n%d\n%.6f\n", n, stored, inHomeAddress, collisions, (float)comparisons / stored);

    int i;
    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        if (hashTable[i]) {
            fprintf(outputFile, "%d %s %d %s %d\n", i, hashTable[i], hashFunction(hashTable[i]),
                    hashFunction(hashTable[i]) == i ? "YES" : "NO", 1);
        } else {
            fprintf(outputFile, "%d --- --- --- ---\n", i);
        }
    }
    fclose(outputFile);

    for (i = 0; i < MAX_TABLE_SIZE; i++) {
        free(hashTable[i]);
    }
    free(hashTable);
    return EXIT_SUCCESS;
}
