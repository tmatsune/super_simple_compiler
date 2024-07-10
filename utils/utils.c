#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void endl(void) { printf("\n"); }

// --------------------------- dynamic array  ------------------------ //

void array_init(dynamic_array *array){
    array->count = 0;
    array->max_count = 5;
    array->items = malloc(array->max_count * sizeof(void *));
    if(array->items == NULL){
        printf("ERR array init: memory alloc\n");
        exit(EXIT_FAILURE);
    }
}

void array_append(dynamic_array *array, const void *item, size_t item_size){
    if(array->count >= array->max_count){
        array->max_count *= 2;
        array->items = realloc(array->items, array->max_count * sizeof(void *));
        if(array->items == NULL){
            printf("ERR array append: memory alloc\n");
            exit(EXIT_FAILURE);
        }
    }
    void *new_item = malloc(item_size);
    if (new_item == NULL) {
        printf("ERR creating new item: memory alloc\n");
        exit(EXIT_FAILURE);
    }
    memcpy(new_item, item, item_size); // to, from, size
    array->items[array->count] = new_item;
    array->count++;
}
void *array_pop(dynamic_array *array){
    if(array->count == 0){
        printf("cannot pop from array is empty\n");
        exit(EXIT_FAILURE);
    }
    void *item = array->items[array->count - 1];
    array->count--;
    return item;
}
void *array_get(dynamic_array *array, size_t index){
    if(index < 0 || index >= array->count){
        printf("index out of range\n");
        exit(EXIT_FAILURE);
    }
    return array->items[index];
}
void array_free(dynamic_array *array){
    for(int i = 0; i < array->count; i++){
        free(array->items[i]);
    }
    free(array->items);
    array->count = 0;
    array->max_count = 0;
}


// --------------------------- string slice ------------------------ //

void slice_init(string_slice *slice){
    slice->count = 0;
    slice->max_count = 1;
    slice->string = malloc(slice->max_count * sizeof(char*));
    if(slice->string == NULL){
        printf("string slice error alloc memory");
        exit(EXIT_FAILURE);
    }
}
void slice_append(string_slice *slice, const char chr){
    slice->max_count++;
    slice->string = realloc(slice->string, slice->max_count * sizeof(char*));
    if(slice->string == NULL){
        printf("string slice error alloc memory");
        exit(EXIT_FAILURE);
    }
    slice->string[slice->count] = chr;
    slice->count++;
}

char *slice_get(string_slice *slice){
    if(slice->count == 0){
        printf("cannot return empty string");
        exit(EXIT_FAILURE);
    }
    return slice->string;
};

void slice_free(string_slice *slice){
    free(slice->string);
    slice->count = 0;
    slice->max_count = 0;
}

// --------------------------- read file ------------------------ //

char *get_file(char *file_name, int *buffer_size){
    FILE* file = fopen(file_name, "r");
    if(file == NULL){
        printf("ERR: could not open file");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);


    char *buffer = (char *)malloc(file_size + 1);
    if(!buffer){
        printf("ERR: buffer empty, error malloc error");
        exit(EXIT_FAILURE);
    }
    size_t read_size = fread(buffer, 1, file_size, file);
    *buffer_size = (int)read_size;
    
    if(read_size != file_size){
        perror("Err reading file");
        free(buffer);
        fclose(file);
        return NULL;
    }
    buffer[file_size] = '\0';
    fclose(file);

    return buffer;
}
