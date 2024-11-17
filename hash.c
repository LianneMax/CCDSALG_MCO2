#include "hash.h"

// Hash function (example using a simple polynomial rolling hash)
unsigned int hashFunction(const char *key) {
    unsigned int hash = 0, p = 31, m = MAX_TABLE_SIZE;
    int i;

    for (i = 0; key[i] != '\0'; i++) {
        hash = (hash * p + key[i]) % m;
    }
    return hash;
}

// Linear probing as a collision resolution technique
unsigned int collisionResolution(unsigned int index, unsigned int attempt) {
    return (index + attempt) % MAX_TABLE_SIZE;
}

// Search function
int search(const char *key, char *table[], unsigned int size) {
    unsigned int index = hashFunction(key);
    unsigned int attempt = 0;
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
void insert(const char *key, char *table[], unsigned int size) {
    unsigned int index = hashFunction(key);
    unsigned int attempt = 0;
    while (table[index] != NULL) {
        if (strcmp(table[index], key) == 0) {
            return; // Duplicate, do not insert
        }
        attempt++;
        index = collisionResolution(index, attempt);
    }
    table[index] = strdup(key); // Store a copy of the key
}