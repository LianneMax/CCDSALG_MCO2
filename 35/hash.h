#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define maximum string length
#define MAX_STRING_LENGTH 15



// Function prototypes
unsigned int custom_hash(const char *str,  int table_size);
int resolve_collision( int index,  int i,  int table_size);
int insert(char *table[],  int table_size, const char *str,  int *collisions);
void search(char *table[],  int table_size, const char *str,  int *comparisons);