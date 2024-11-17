#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 18031 // Adjust based on the prime number logic
#define MAX_STRING_LENGTH 15

// Function Prototypes
int hashFunction(const char *key);
int collisionResolution(int index, int attempt);
int search(const char *key, char *table[], int size);
void insert(const char *key, char *table[], int size);

#endif // HASH_H
