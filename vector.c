#include "vector.h"
#include "logger.h"

#include <stdlib.h>

bool debug = false;

static void int_vector_memset(IntVector *vector, int value, size_t start)
{
    if (!vector->data) {
        logger(ERROR, debug, __func__, "Vector is NOT properly initialized.");
        return;
    }

    logger(
        INFO, debug, __func__,
        "Setting value %i to vector: %p starting from: %li...",
        value, vector, start
    );

    for (size_t i = start; i < vector->size; ++i)
        vector->data[i] = value;

    logger(INFO, debug, __func__, "Memory set.");
}

static void int_vector_allocate(IntVector *vector)
{
    logger(
        INFO, debug, __func__,
        "Allocating memory to be able to hold %li numbers in vector: %p...",
        vector->size, vector
    );

    vector->data = (int *) malloc(vector->size * sizeof(int));
    logger(
        INFO, debug, __func__,
        "%li spaces in memory allocated for vector: %p.",
        vector->size, vector
    );
}

void int_vector_resize(IntVector *vector, size_t newSize)
{
    if (newSize == -1) {
        newSize = vector->size + DEFAULT_RESIZE_VALUE;
    } else {
        newSize += vector->size;
    }
    
    size_t oldSize = vector->size;

    logger(
        INFO, debug, __func__,
        "Resizing vector: %p, old size: %li, new size: %li...",
        vector, vector->size, newSize
    );

    logger(INFO, debug, __func__, "Saving vector contents to avoid data loss...");
    int temp[oldSize];
    for (size_t i = 0; i < oldSize; ++i) {
        temp[i] = vector->data[i];
    }

    logger(
        INFO, debug, __func__,
        "Vector contents saved. Freeing vector: %p, and re-initializing it with new size.",
        vector
    );

    int_vector_free(vector);
    int_vector_init(vector, newSize);

    logger(INFO, debug, __func__, "Restoring vector contents...");
    for (size_t i = 0; i < oldSize; ++i) {
        vector->data[i] = temp[i];
        ++vector->offset;
    }
    logger(INFO, debug, __func__, "Vector contents restored. Setting value 0 to the rest of the vector...");
    int_vector_memset(vector, 0, oldSize);

    logger(
        INFO, debug, __func__,
        "Vector: %p resized. New size: %li",
        vector, vector->size
    );
}

void int_vector_set_debug(bool value)
{
    debug = value;
}

void int_vector_init(IntVector *vector, size_t initialSize)
{
    if (initialSize == -1)
        initialSize = DEFAULT_RESIZE_VALUE;
    logger(INFO, debug, __func__, "Initializing vector: %p with size: %li", vector, initialSize);

    vector->size = initialSize;
    vector->offset = 0;

    int_vector_allocate(vector);
    int_vector_memset(vector, 0, 0);
}

void int_vector_shrink(IntVector *vector)
{
    logger(
        INFO, debug, __func__,
        "Shrinking vector: %p, memory allocated: %li, actual size: %li",
        vector, vector->size, vector->offset
    );

    size_t newSize = vector->offset;
    int temp[newSize];

    logger(
        INFO, debug, __func__,
        "Saving vector contents to avoid data loss..."
    );

    for (size_t i = 0; i < newSize; ++i) {
        temp[i] = vector->data[i];
    }

    logger(
        INFO, debug, __func__,
        "Vector contents saved in temporal array: %p.", temp
    );

    logger(
        INFO, debug, __func__,
        "Freeing vector: %p and re-initializing it with the actual size...",
        vector
    );

    int_vector_free(vector);
    int_vector_init(vector, newSize);
    logger(
        INFO, debug, __func__,
        "Vector: %p re-initialized. Restoring contents...",
        vector
    );

    for (size_t i = 0; i < newSize; ++i) {
        vector->data[i] = temp[i];
    }

    logger(
        INFO, debug, __func__,
        "Contents restored on vector: %p. All done!",
        vector
    );
}

void int_vector_add_int(IntVector *vector, int value)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p hasn't been properly initialized. Please call int_vector_init() before using this function.",
            vector
        );
        return;
    }

    logger(
        INFO, debug, __func__,
        "Adding value: %i to vector: %p...",
        value, vector
    );

    if (vector->offset == vector->size) {
        logger(
            INFO, debug, __func__,
            "Adding new value to vector causes it to be resized."
        );
        int_vector_resize(vector, -1);
    }

    vector->data[vector->offset++] = value;
}

void int_vector_add_int_array(IntVector *vector, int array[], size_t arraySize)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p hasn't been properly initialized. Please call int_vector_init() before using this function.",
            vector
        );
        return;
    }

    if (arraySize > vector->size || (vector->offset + arraySize) > vector->size) {
        logger(
            INFO, debug, __func__,
            "Adding new values to vector causes it to be resized."
        );
        int_vector_resize(vector, arraySize);
    }

    for (size_t i = 0; i < arraySize; ++i) {
        vector->data[vector->offset++] = array[i];
    }
}

void int_vector_copy(const IntVector *source, IntVector *dest)
{
    logger(
        INFO, debug, __func__,
        "Copying vector contents from vector: %p to vector: %p...",
        source, dest
    );

    if (dest->size < source->offset) {
        logger(INFO, debug, __func__, "Destination vector isn't big enough, resizing...");
        int_vector_resize(dest, source->offset - dest->size);
    }

    for (size_t i = 0; i < source->offset; ++i) {
        dest->data[dest->offset++] = source->data[i];
    }

    logger(INFO, debug, __func__, "Vector copied.");
}

void int_vector_free(IntVector *vector)
{
    logger(INFO, debug, __func__, "Freeing vector: %p.", vector);
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->offset = 0;
    logger(INFO, debug, __func__, "Vector: %p freed.", vector);
}

void int_vector_print(const IntVector *vector)
{
    for (size_t i = 0; i < vector->size; ++i) {
        printf("%i%s", vector->data[i], (i == vector->size - 1 ? "\n" : ", "));
    }
}

int int_vector_get_at(const IntVector *vector, size_t index)
{
    if (index >= vector->size || index < 0) {
        logger(
            ERROR, true, __func__,
            "Index: %li is out of bound.", index
        );
        return -1;
    }

    return vector->data[index];
}

int int_vector_get_last(const IntVector *vector)
{
    return int_vector_get_at(vector, vector->offset - 1);
}
