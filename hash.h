#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum string length
#define MAX_STRING_LENGTH 15

// Function prototypes
unsigned int hash_function(const char *str, unsigned int table_size);
unsigned int resolve_collision(unsigned int index, unsigned int i, unsigned int table_size);
int insert(char *table[], unsigned int table_size, const char *str, unsigned int *collisions);
int search(char *table[], unsigned int table_size, const char *str, unsigned int *comparisons);
void free_table(char *table[], unsigned int table_size);

#endif // HASH_H
