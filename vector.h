#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdio.h>

#define DEFAULT_RESIZE_VALUE 10
#define DEFAULT_STRING_SIZE 63

typedef struct {
    int *data;
    size_t size;
    size_t offset;
} IntVector;

typedef struct {
    size_t vector_size;
    size_t offset;
    size_t *allocated_sizes;
    size_t *actual_sizes;
    char **data;
} StringVector;

void set_debug(bool value);
void int_vector_resize(IntVector *vector, size_t new_size);
void int_vector_shrink(IntVector *vector);
void int_vector_init(IntVector *vector, size_t initial_size);
void int_vector_add(IntVector *vector, int value);
void int_vector_add_array(IntVector *vector, const int array[], size_t array_size);
void int_vector_copy(const IntVector *source, IntVector *dest);
void int_vector_free(IntVector *vector);
void int_vector_print(const IntVector *vector);
int int_vector_get_at(const IntVector *vector, const size_t index);
int int_vector_get_last(const IntVector *vector);

size_t string_vector_strlen(const char *value);
void string_vector_init(StringVector *vector, size_t vector_size, size_t items_size);
void string_vector_strcpy(const StringVector *vector, const char *src, char *dest, const size_t n);
void string_vector_resize(StringVector *vector, size_t new_size);
void string_vector_free(StringVector *vector);
void string_vector_shrink(StringVector *vector);
void string_vector_shrink_items(StringVector *vector);
void string_vector_add(StringVector *vector, const char *value);
void string_vector_print(StringVector *vector);
char *string_vector_get_at(const StringVector *vector, const size_t index);
char *string_vector_get_last(const StringVector *vector);

#endif // VECTOR_H
