#include "vector.h"
#include "logger.h"

#include <stdlib.h>

bool debug = false;

void set_debug(bool value)
{
    debug = value;
}

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

void int_vector_resize(IntVector *vector, size_t new_size)
{
    if (new_size == -1) {
        new_size = vector->size + DEFAULT_RESIZE_VALUE;
    } else {
        new_size += vector->size;
    }
    
    size_t old_size = vector->size;

    logger(
        INFO, debug, __func__,
        "Resizing vector: %p, old size: %li, new size: %li...",
        vector, vector->size, new_size
    );

    logger(INFO, debug, __func__, "Saving vector contents to avoid data loss...");
    int temp[old_size];
    for (size_t i = 0; i < old_size; ++i) {
        temp[i] = vector->data[i];
    }

    logger(
        INFO, debug, __func__,
        "Vector contents saved. Freeing vector: %p, and re-initializing it with new size.",
        vector
    );

    int_vector_free(vector);
    int_vector_init(vector, new_size);

    logger(INFO, debug, __func__, "Restoring vector contents...");
    for (size_t i = 0; i < old_size; ++i) {
        vector->data[i] = temp[i];
        ++vector->offset;
    }
    logger(INFO, debug, __func__, "Vector contents restored. Setting value 0 to the rest of the vector...");
    int_vector_memset(vector, 0, old_size);

    logger(
        INFO, debug, __func__,
        "Vector: %p resized. New size: %li",
        vector, vector->size
    );
}

void int_vector_init(IntVector *vector, size_t initial_size)
{
    if (initial_size == -1)
        initial_size = DEFAULT_RESIZE_VALUE;
    logger(INFO, debug, __func__, "Initializing vector: %p with size: %li", vector, initial_size);

    vector->size = initial_size;
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

    size_t new_size = vector->offset;
    int temp[new_size];

    logger(
        INFO, debug, __func__,
        "Saving vector contents to avoid data loss..."
    );

    for (size_t i = 0; i < new_size; ++i) {
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
    int_vector_init(vector, new_size);
    logger(
        INFO, debug, __func__,
        "Vector: %p re-initialized. Restoring contents...",
        vector
    );

    for (size_t i = 0; i < new_size; ++i) {
        vector->data[i] = temp[i];
    }

    logger(
        INFO, debug, __func__,
        "Contents restored on vector: %p. All done!",
        vector
    );
}

void int_vector_add(IntVector *vector, int value)
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

void int_vector_add_array(IntVector *vector, const int array[], size_t array_size)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p hasn't been properly initialized. Please call int_vector_init() before using this function.",
            vector
        );
        return;
    }

    if (array_size > vector->size || (vector->offset + array_size) > vector->size) {
        logger(
            INFO, debug, __func__,
            "Adding new values to vector causes it to be resized."
        );
        int_vector_resize(vector, array_size);
    }

    for (size_t i = 0; i < array_size; ++i) {
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

static void string_vector_allocate(StringVector *vector, size_t items_size)
{
    logger(
        INFO, debug, __func__,
        "Allocating memory to be able to hold %li strings with size: %li in vector: %p...",
        vector->vector_size, items_size, vector
    );

    vector->data = (char **) malloc(vector->vector_size * sizeof(char *));
    vector->allocated_sizes = (size_t *) malloc(vector->vector_size * sizeof(size_t));
    vector->actual_sizes = (size_t *) malloc(vector->vector_size * sizeof(size_t));

    logger(
        INFO, debug, __func__,
        "%li spaces in memory allocated for vector: %p.",
        vector->vector_size, vector
    );

    if (items_size > 0) {
        logger(INFO, debug, __func__, "Initializing vector items...");
        for (size_t i = 0; i < vector->vector_size; ++i) {
            vector->allocated_sizes[i] = items_size * sizeof(char) + 1;
            vector->data[i] = (char *) malloc(vector->allocated_sizes[i]);

            if (!vector->data[i]) {
                logger(
                    ERROR, true, __func__,
                    "There was an error while initializing vector item: %p with size: %li.",
                    vector, vector->allocated_sizes[i]
                );
                return;
            }

            logger(
                INFO, debug, __func__,
                "Vector item: %p initialized.",
                vector->data[i]
            );
        }
        logger(INFO, debug, __func__, "All vector items were initialized.");
    }
}

size_t string_vector_strlen(const char *value)
{
    size_t size = 0;
    while (value[size] != '\0')
        ++size;
    return size;
}

static void string_vector_memset(StringVector *vector, char value, size_t start)
{
    if (!vector->data) {
        logger(
            ERROR, debug, __func__,
            "Vector: %p is NOT properly initialized! Please call string_vector_init() before using this function.",
            vector
        );
        return;
    }

    logger(
        INFO, debug, __func__,
        "Setting value: %c to vector: %p starting from: %li...",
        value, vector, start
    );

    for (size_t i = start; i < vector->vector_size; ++i) {
        size_t j;
        for (j = 0; j < vector->allocated_sizes[i]; ++j) {
            vector->data[i][j] = value;
        }
        vector->data[i][j] = '\0';
    }

    logger(
        INFO, debug, __func__,
        "Memory set."
    );
}

void string_vector_free(StringVector *vector)
{
    logger(INFO, debug, __func__, "Starting to free items in vector: %p...", vector);
    for (size_t i = 0; i < vector->vector_size; ++i) {
        logger(INFO, debug, __func__, "Freeing item: %p in vector: %p...", vector->data[i], vector);
        free(vector->data[i]);

        logger(INFO, debug, __func__, "Item: %p in vector: %p freed.", vector->data[i], vector);
        vector->data[i] = NULL;
    }

    logger(INFO, debug, __func__, "All items in vector: %p were freed. Freeing vector...", vector);
    free(vector->data);
    free(vector->allocated_sizes);
    free(vector->actual_sizes);
    logger(INFO, debug, __func__, "Vector: %p freed.", vector);

    vector->data = NULL;
    vector->allocated_sizes = NULL;
    vector->actual_sizes = NULL;
    vector->vector_size = 0;
    vector->offset = 0;
}

void string_vector_shrink(StringVector *vector)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p is NOT properly initialized. Please call string_vector_init() and string_vector_add() before using this function.",
            vector
        );
        return;
    }

    logger(
        INFO, debug, __func__,
        "Starting to shrink vector: %p. Saving vector contents to avoid data loss...",
        vector
    );

    size_t actual_vector_size = vector->offset;
    char *temp[actual_vector_size];
    size_t sizes[actual_vector_size];

    for (size_t i = 0; i < actual_vector_size; ++i) {
        sizes[i] = vector->actual_sizes[i];
        temp[i] = (char *) malloc(sizes[i] * sizeof(char) + 1);
        for (size_t j = 0; j < sizes[i]; ++j) {
            temp[i][j] = ' ';
        }
    }

    for (size_t i = 0; i < vector->offset; ++i) {
        logger(INFO, debug, __func__, "Saving vector item: %s...", vector->data[i]);
        string_vector_strcpy(vector, vector->data[i], temp[i], sizes[i]);
    }

    logger(
        INFO, debug, __func__,
        "Vector contents saved. Shrinking vector..."
    );

    string_vector_free(vector);
    string_vector_init(vector, actual_vector_size, -1);

    logger(
        INFO, debug, __func__,
        "Vector: %p shrinked. Restoring contents...",
        vector
    );

    for (size_t i = 0; i < actual_vector_size; ++i) {
        vector->actual_sizes[i] = sizes[i];
        string_vector_strcpy(vector, temp[i], vector->data[i], vector->actual_sizes[i]);
        ++vector->offset;
        free(temp[i]);
    }

    logger(
        INFO, debug, __func__,
        "Vector contents restored. All done!"
    );
}

void string_vector_shrink_items(StringVector *vector)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p is NOT properly initialized. Please call string_vector_init() and string_vector_add() before using this function.",
            vector
        );
        return;
    }

    logger(
        INFO, debug, __func__,
        "Starting to shrink vector items. Saving item contents to avoid data loss..."
    );

    for (size_t i = 0; i < vector->offset; ++i) {
        size_t item_size = string_vector_strlen(vector->data[i]);
        char temp[item_size];

        // Set an empty character to the whole string for string_vector_strlen(), which is called by string_vector_strcpy(), to work correctly.
        for (size_t j = 0; j < item_size; ++j) {
            temp[j] = ' ';
        }

        string_vector_strcpy(vector, vector->data[i], temp, item_size);

        logger(INFO, debug, __func__, "Item contents saved. Freeing item and re-initializing it with its actual size...");
        free(vector->data[i]);
        logger(INFO, debug, __func__, "Item: %p freed. Allocating string actual size...", vector->data[i]);

        vector->data[i] = NULL;
        vector->data[i] = (char *) malloc(item_size * sizeof(char));
        logger(
            INFO, debug, __func__,
            "Allocated %li bytes to vector item: %p. Setting default value to the whole string...",
            item_size, vector->data[i]
        );

        for (size_t j = 0; j < item_size; ++j) {
            vector->data[i][j] = ' ';
        }

        logger(INFO, debug, __func__, "Item shrinked. Restoring contents...");
        string_vector_strcpy(vector, temp, vector->data[i], item_size);
    }

    logger(
        INFO, debug, __func__,
        "Vector items shrinked!"
    );
}

size_t string_vector_get_vector_item_pos(const StringVector *vector, char **item)
{
    size_t pos = -1;
    for (size_t i = 0; i < vector->vector_size; ++i) {
        if (vector->data[i] == *item) {
            pos = i;
            break;
        }
    }
    return pos;
}

/* vector is just for this function to know which item _item_ points to inside vector,
 * so we can make vector->item_size[item] as long as its new size. */
static void string_vector_item_resize(const StringVector *vector, char **item, size_t new_size, bool save_contents)
{
    size_t old_size = string_vector_strlen(*item);
    if (new_size == -1) {
        new_size = old_size + DEFAULT_STRING_SIZE;
    } else {
        new_size += old_size;
    }

    logger(
        INFO, debug, __func__,
        "Resizing vector item: %p, old size: %li, new size: %li...",
        item, old_size, new_size
    );

    char *temp;
    if (save_contents) {
        logger(
            INFO, debug, __func__,
            "Saving item contents to avoid data loss..."
        );

        temp = (char *) malloc(old_size * sizeof(char));
        string_vector_strcpy(vector, *item, temp, old_size);

        logger(INFO, debug, __func__, "Item contents saved. Freing item: %p and re-initializing it with new size...", item);
    } else {
        logger(INFO, debug, __func__, "Freeing item: %p and re-initializing it with new size...", item);
    }

    size_t pos = string_vector_get_vector_item_pos(vector, item);
    vector->allocated_sizes[pos] = new_size * sizeof(char) + 1; 

    free(*item);
    *item = (char *) malloc(vector->allocated_sizes[pos]);

    if (save_contents) {
        logger(INFO, debug, __func__, "Restoring contents...");
        for (size_t i = 0; i < old_size; ++i) {
            *item[i] = temp[i];
        }
        free(temp);
    }

    logger(INFO, debug, __func__, "Setting default value for the rest of the item...");
    size_t start = save_contents ? old_size : 0;
    for (size_t i = start; i < new_size; ++i) {
        (*item)[i] = ' ';
    }

    logger(INFO, debug, __func__, "Vector item: %p resized.", item);
}

/* vector is needed because this function may use string_vector_resize(). */
void string_vector_strcpy(
    const StringVector *vector,
    const char *src,
    char *dest,
    const size_t n)
{
    logger(
        INFO, debug, __func__,
        "Copying %li individual characters of string: %s held by pointer: %p into pointer: %p...",
        n, src, src, dest
    );

    size_t src_size = string_vector_strlen(src);
    size_t dest_size = string_vector_strlen(dest);
    logger(INFO, debug, __func__, "Source size is: %li, destination size is: %li.", src_size, dest_size);

    if (src_size > dest_size) {
        logger(INFO, debug, __func__, "Destination pointer isn't as big as source. Resizing...");
        string_vector_item_resize(vector, &dest, n, false);
        logger(INFO, debug, __func__, "Destination pointer resized. Continuing...");
    }

    size_t i;
    for (i = 0; i < n; ++i) {
        dest[i] = src[i];
    }
    dest[i] = '\0';

    logger(
        INFO, debug, __func__,
        "Vector item contents copied."
    );
}

void string_vector_resize(StringVector *vector, size_t new_size)
{
    size_t old_size = vector->vector_size;
    if (new_size == -1) {
        new_size = old_size + DEFAULT_RESIZE_VALUE;
    } else {
        new_size += old_size;
    }

    logger(
        INFO, debug, __func__,
        "Resizing vector: %p, old size: %li, new size: %li...",
        vector, old_size, new_size
    );

    logger(
        INFO, debug, __func__,
        "Saving vector contents to avoid data loss..."
    );

    char *temp[old_size];
    size_t sizes[old_size];

    for (size_t i = 0; i < old_size; ++i) {
        sizes[i] = vector->actual_sizes[i];
        temp[i] = (char *) malloc(sizes[i] * sizeof(char) + 1);
        // This assignment here is for string_vector_strlen() which string_vector_strcpy() calls to work.
        for (size_t j = 0; j < sizes[i]; ++j) {
            temp[i][j] = ' ';
        }
        string_vector_strcpy(vector, vector->data[i], temp[i], sizes[i]);
    }

    logger(
        INFO, debug, __func__,
        "Vector contents saved. Freeing vector: %p, and re-initializing it with new size...",
        vector
    );

    string_vector_free(vector);
    string_vector_init(vector, new_size, -1);

    for (size_t i = 0; i < new_size; ++i) {
        vector->allocated_sizes[i] = sizes[i];
        vector->data[i] = (char *) malloc(vector->allocated_sizes[i] * sizeof(char) + 1);
    }

    logger(INFO, debug, __func__, "Restoring contents...");
    for (size_t i = 0; i < old_size; ++i) {
        vector->actual_sizes[i] = sizes[i];
        string_vector_strcpy(vector, temp[i], vector->data[i], vector->actual_sizes[i]);
        ++vector->offset;
    }

    logger(
        INFO, debug, __func__,
        "Vector contents restored."
    );
}

void string_vector_init(StringVector *vector, size_t vector_size, size_t items_size)
{
    if (vector_size == -1) {
        vector_size = DEFAULT_RESIZE_VALUE;
    }

    if (items_size == -1)
        items_size = DEFAULT_STRING_SIZE;

    logger(
        INFO, debug, __func__,
        "Initializing vector: %p with initial vector size: %li, initial vector items size: %li...",
           vector, vector_size, items_size
    );

    vector->vector_size = vector_size;
    vector->offset = 0;
    string_vector_allocate(vector, items_size);
    string_vector_memset(vector, ' ', 0);
}

void string_vector_add(StringVector *vector, const char *value)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p is NOT properly initialized! Please call string_vector_init() before using this function.",
            vector
        );
        return;
    }

    logger(
        INFO, debug, __func__,
        "Adding value: %s to vector: %p...",
        value, vector
    );

    if (vector->offset == vector->vector_size) {
        logger(
            INFO, debug, __func__,
            "Adding new value causes resizing."
        );
        string_vector_resize(vector, -1);
    }

    size_t value_size = string_vector_strlen(value);

    vector->actual_sizes[vector->offset] = value_size;
    string_vector_strcpy(vector, value, vector->data[vector->offset], value_size);
    ++vector->offset;

    logger(
        INFO, debug, __func__,
        "Value: %s added to vector: %p.",
        value, vector
    );
}

static bool string_vector_is_empty(const char *value)
{
    size_t value_size = string_vector_strlen(value);
    for (size_t i = 0; i < value_size; ++i) {
        if (value[i] != ' ')
            return false;
    }
    return true;
}

void string_vector_print(StringVector *vector)
{
    if (!vector->data) {
        logger(
            ERROR, true, __func__,
            "Vector: %p is NOT properly initialized. Please call string_vector_init() and string_vector_add() before using this function.",
            vector
        );
        return;
    }

    for (size_t i = 0; i < vector->vector_size; ++i) {
        if (string_vector_is_empty(vector->data[i]))
            continue;
        printf("%s\n", vector->data[i]);
    }
}

// Returns true if index is valid, otherwise write a log telling the user index is not valid.
static bool check_index(const StringVector *vector, const size_t index)
{
    if (index < 0 || index >= vector->vector_size) {
        logger(ERROR, true, __func__, "Index: %li is out of bound.", index);
        return false;
    }
    return true;
}

char *string_vector_get_at(const StringVector *vector, const size_t index)
{
    if (check_index(vector, index))
        return vector->data[index];
    return NULL;
}

char *string_vector_get_last(const StringVector *vector)
{
    return string_vector_get_at(vector, vector->offset - 1);
}
