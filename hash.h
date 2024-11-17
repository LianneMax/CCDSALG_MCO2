#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_TABLE_SIZE 18031 // Adjust based on the example's prime number logic
#define MAX_STRING_LENGTH 15

// Function Prototypes
unsigned int hashFunction(const char *key);
unsigned int collisionResolution(unsigned int index, unsigned int attempt);
int search(const char *key, char *table[], unsigned int size);
void insert(const char *key, char *table[], unsigned int size);

#endif // HASH_H