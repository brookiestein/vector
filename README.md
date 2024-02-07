# Vector
Vector C Library. This library allows you to have a dynamically allocated array working as a vector,
e.g. std::vector from the C++'s STL.

It automatically detects when it needs more memory to be allocated in order to hold the data on the fly.

For example, say we initially want to have an array of 5 ints:

```
IntVector numbers;
int_vector_init(&numbers, 5);
```
**After making a vector variable, you have to initialize it with the function `int_vector_init(vector, size)`.**

We start adding numbers to our array:
```
for (size_t i = 0; i < 5; ++i)
    int_vector_add_int(&numbers, i + 1);
```

_After walking the streets of our city_, we realize we need to hold 3 more numbers.
Do we need to delete that _numbers_ array and/or make another one which would be able to hold 8 ints?

What we need to do is just call `int_vector_add_int()` 3 more times to add those 3 more numbers, and _the magic_ 
is done under the hood without needing you to worry about memory allocation.
```
int last = int_vector_get_last(&numbers);
for (size_t i = 0; i < 3; ++i)
    int_vector_add_int(&numbers, last + i + 1);
```

That's it!

If at some point you want to know what's going on like, in this example, you can call `set_debug(true)`
and you will see a message telling you that:
```
Info: int_vector_add_int(): Adding value: 6 to vector: 0x7fffee8b2848...
Info: int_vector_add_int(): Adding new value to vector causes it to be resized.
Info: int_vector_resize(): Resizing vector: 0x7fffee8b2848, old size: 5, new size: 15...
```

Vector is automatically resized to hold 10 more ints, so vector's size will now be 15.

If at some point you consider you have _too much_ memory allocated than items in the vector, just call `int_vector_shrink(&numbers)`.

It'll shrink the vector to have just 32 bytes of memory allocated (in a 64-bit system, which would have sizeof(int) == 4),
which is the actual size of the vector internal array: 8 * 4.
