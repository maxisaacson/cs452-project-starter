#include <stdio.h>
#include <stdlib.h>
#include "../src/lab.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("usage: %s <array_size> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int size = atoi(argv[1]);
    int t = atoi(argv[2]);

    if (size <= 0 || t <= 0) {
        fprintf(stderr, "Error: array_size and num_threads must be positive integers.\n");
        return EXIT_FAILURE;
    }

    int *A_ = malloc(sizeof(int) * size);
    if (!A_) {
        fprintf(stderr, "Memory allocation failed for array A_\n");
        return EXIT_FAILURE;
    }

    // Initialize the array with random values
    srandom(1);
    for (int i = 0; i < size; i++) {
        A_[i] = random() % 100000;
    }

    double start = getMilliSeconds();
    mergesort_mt(A_, size, t);
    double end = getMilliSeconds();

    printf("%f %d\n", end - start, t);

    free(A_);
    return EXIT_SUCCESS;
}
