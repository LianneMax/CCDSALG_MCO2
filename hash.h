#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum string length and hash table size
#define MAX_STRING_LENGTH 15
#define HASH_TABLE_SIZE 18031 // First prime number above 1.1 * 16384

// Function prototypes
int hash_function(const char *str, int table_size);
int resolve_collision(int index, int i, int table_size);
int insert(char *table[], int table_size, const char *str, int *collisions);
int search(char *table[], int table_size, const char *str, int *comparisons);
void free_table(char *table[], int table_size);

#endif
