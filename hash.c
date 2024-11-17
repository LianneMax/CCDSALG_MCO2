#include "hash.h"

// Hash function (example using a simple polynomial rolling hash)
int hashFunction(const char *key) {
    int hash = 0, p = 31, m = MAX_TABLE_SIZE;
    int i; // Declare i outside the for loop
    for (i = 0; key[i] != '\0'; i++) {
        hash = (hash * p + key[i]) % m;
        if (hash < 0) { 
            hash += m; // Ensure non-negative index
        }
    }
    return hash;
}

// Linear probing as a collision resolution technique
int collisionResolution(int index, int attempt) {
    int newIndex = (index + attempt) % MAX_TABLE_SIZE;
    if (newIndex < 0) {
        newIndex += MAX_TABLE_SIZE; // Ensure non-negative index
    }
    return newIndex;
}

// Search function
int search(const char *key, char *table[], int size) {
    int index = hashFunction(key);
    int attempt = 0;
    while (table[index] != NULL) {
        if (strcmp(table[index], key) == 0) {
            return index;
        }
        attempt++;
        index = collisionResolution(index, attempt);
    }
    return -1; // Not found
}

// Insert function
void insert(const char *key, char *table[], int size) {
    int index = hashFunction(key);
    int attempt = 0;
    while (table[index] != NULL) {
        if (strcmp(table[index], key) == 0) {
            return; // Duplicate, do not insert
        }
        attempt++;
        index = collisionResolution(index, attempt);
        if (attempt >= size) {
            fprintf(stderr, "Hash table is full!\n");
            return; // Table is full
        }
    }
    table[index] = strdup(key); // Store a copy of the key
}
