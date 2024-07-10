#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>

void endl(void);

// ---------------- dynamic array ---------------- //

typedef struct{
    void **items;
    int count;
    int max_count;
} dynamic_array;

void array_init(dynamic_array *array);
void array_append(dynamic_array *array, const void *item, size_t item_size);
void *array_pop(dynamic_array *array);
void *array_get(dynamic_array *array, size_t index);
void array_free(dynamic_array *array);


// ---------------- string slice ---------------- //

typedef struct {
    char *string;
    int count;
    int max_count;
} string_slice;

void slice_init(string_slice *slice);
void slice_append(string_slice *slice, const char chr);
char *slice_get(string_slice *slice);
void slice_free(string_slice *slice);

char *get_file(char *file_name, int *file_size);


#endif