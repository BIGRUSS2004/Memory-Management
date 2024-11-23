#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void parse_file(FILE *f, int input[][2], int *n, int *partition_size) {
    fscanf(f, "%d", partition_size); // Read initial partition size
    while (fscanf(f, "%d %d", &input[*n][0], &input[*n][1]) != EOF) {
        (*n)++;
    }
}

void get_input(char *args[], int input[][2], int *n, int *partition_size, int *policy) {
    FILE *input_file = fopen(args[1], "r");
    if (!input_file) {
        fprintf(stderr, "Error: Invalid filepath\n");
        exit(EXIT_FAILURE);
    }

    parse_file(input_file, input, n, partition_size);
    fclose(input_file);

    if (args[2][0] == '-' && args[2][1] == 'F') {
        *policy = 1; // FIFO
    } else if (args[2][0] == '-' && args[2][1] == 'B') {
        *policy = 2; // Best Fit
    } else if (args[2][0] == '-' && args[2][1] == 'W') {
        *policy = 3; // Worst Fit
    } else {
        fprintf(stderr, "Error: Invalid policy. Use -F, -B, or -W.\n");
        exit(EXIT_FAILURE);
    }
}
