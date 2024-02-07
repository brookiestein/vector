#include "vector.h"

int main(int argc, char *argv[])
{
    set_debug(true);
    StringVector names;
    string_vector_init(&names, 5, -1);

    string_vector_add(&names, "John");
    string_vector_add(&names, "Alice");
    string_vector_add(&names, "Bob");

    printf("\nVector:\n");
    string_vector_print(&names);
    printf("\n");

    printf("Vector size: %li\n", names.vector_size);
    printf("Items in vector: %li\n\n", names.offset);

    string_vector_shrink(&names);

    printf("\nVector:\n");
    string_vector_print(&names);
    printf("\n");

    printf("Vector size: %li\n", names.vector_size);
    printf("Items in vector: %li\n\n", names.offset);

    string_vector_free(&names);
}
