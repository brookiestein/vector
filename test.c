#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    printf("Testing functionality...\n\n");

    int_vector_set_debug(true);
    IntVector numbers;
    int_vector_init(&numbers, 5);

    int_vector_add_int(&numbers, 10);
    int_vector_add_int(&numbers, 20);
    int_vector_add_int(&numbers, 30);
    int_vector_add_int(&numbers, 40);
    int_vector_add_int(&numbers, 50);

    printf("\nVector:\n");
    int_vector_print(&numbers);
    printf("\n");
    int_vector_add_int(&numbers, 60);
    int_vector_shrink(&numbers);
    printf("\nVector:\n");
    int_vector_print(&numbers);

    printf("\n");
    int other_numbers[] = { 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200 };
    int_vector_add_int_array(&numbers, other_numbers, 14);
    printf("\nVector:\n");
    int_vector_print(&numbers);

    printf("\n");
    IntVector numbers_copy;
    int_vector_init(&numbers_copy, numbers.size);
    int_vector_copy(&numbers, &numbers_copy);
    printf("\nVector (Copy):\n");
    int_vector_print(&numbers_copy);

    printf("\nValue at index -10 of numbers vector: %i\n", int_vector_get_at(&numbers, -10));
    printf("\nValue at index 5 of numbers vector: %i\n", int_vector_get_at(&numbers, 5));
    printf("\nValue at index %li of numbers vector: %i\n", numbers.size, int_vector_get_at(&numbers, numbers.size));
    printf("\nLast number of numbers vector: %i\n", int_vector_get_last(&numbers));

    int_vector_free(&numbers);
    int_vector_free(&numbers_copy);

    if (argc == 1)
        return 0;

    size_t size = atoi(argv[1]);
    if (size == 0) {
        fprintf(stderr, "First argument is not a correct size or you just gave me 0.\n");
        return 0;
    }

    printf("\nAdding %li numbers to vector since user wants to test...\n\n", size);

    IntVector user_test;
    int_vector_init(&user_test, size);
    for (size_t i = 0; i < size; ++i) {
        int_vector_add_int(&user_test, i + 1);
    }

    printf("\n\nUser vector:\n\n");
    int_vector_print(&user_test);
    int_vector_free(&user_test);
}
