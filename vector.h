#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_RESIZE_VALUE 10

typedef struct {
    int *data;
    size_t size;
    size_t offset;
} IntVector;

void int_vector_resize(IntVector *vector, size_t newSize);
void int_vector_set_debug(bool value);
void int_vector_shrink(IntVector *vector);
void int_vector_init(IntVector *vector, size_t initialSize);
void int_vector_add_int(IntVector *vector, int value);
void int_vector_add_int_array(IntVector *vector, int array[], size_t arraySize);
void int_vector_copy(const IntVector *source, IntVector *dest);
void int_vector_free(IntVector *vector);
void int_vector_print(const IntVector *vector);
int int_vector_get_at(const IntVector *vector, size_t index);
int int_vector_get_last(const IntVector *vector);

#endif // VECTOR_H
