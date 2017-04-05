#include <malloc.h>
#include <mem.h>
#include "misc.h"

/**
 * Swaps the values at two pointers
 *
 * @param a Pointer a.
 * @param b Pointer b.
 * @param size Size of value pointed at by a and b.
 */
void swap(void *a, void *b, size_t size) {
    void *temp = malloc(size);
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
    free(temp);
}

/**
 * @param max Maximum value of random integer.
 * @return Random integer [0..max].
 */
unsigned int rand_range(int min, int max) {
    int r;
    int range = (max - min) + 1;
    int buckets = RAND_MAX / range;
    int limit = buckets * range;

    // Ensure fair spread of numbers by excluding some numbers in
    // the upper range of rand() that would create a biased result
    do {
        r = rand();
    } while (r >= limit);

    return min + (unsigned int) (r / buckets);
}

/**
 * Shuffles an array using the Fisher–Yates algorithm
 * @param array Pointer to the first element of the array to be shuffled.
 * @param num Number of elements in the array.
 * @param size Size in bytes of each element in the array.
 */
void shuffle(void *array, size_t num, size_t size) {
    for (size_t i = 0; i < num; i++) {
        size_t j = i + rand_range(0, num - i - 1);

        if (i != j) {
            swap(array + (i * size), array + (j * size), size);
        }
    }
}