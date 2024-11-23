#include "mmu.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("usage: ./mmu <input file> -{F | B | W }\n");
        printf("(F=FIFO | B=BESTFIT | W=WORSTFIT)\n");
        return 1;
    }

    int partition_size, input[200][2], n = 0, policy;
    list_t *free_list = list_alloc();   // List for free memory blocks
    list_t *alloc_list = list_alloc();  // List for allocated memory blocks

    // Parse input file and get memory management policy
    get_input(argv, input, &n, &partition_size, &policy);

    // Create the initial memory partition
    block_t *partition = malloc(sizeof(block_t));
    partition->start = 0;
    partition->end = partition_size - 1;
    partition->pid = 0;  // PID 0 indicates a free block
    list_add_to_front(free_list, partition);

    // Process each command in the input
    int i; // Moved declaration outside the loop
    for (i = 0; i < n; i++) {
        printf("************************\n");
        if (input[i][0] > 0) {  // Allocate memory
            printf("ALLOCATE: %d FROM PID: %d\n", input[i][1], input[i][0]);
            allocate_memory(free_list, alloc_list, input[i][0], input[i][1], policy);
        } else if (input[i][0] < 0 && input[i][0] != -99999) {  // Deallocate memory
            printf("DEALLOCATE MEM: PID %d\n", -input[i][0]);
            deallocate_memory(alloc_list, free_list, -input[i][0], policy);
        } else if (input[i][0] == -99999) {  // Coalesce memory
            printf("COALESCE/COMPACT\n");
            free_list = coalese_memory(free_list);
        }
        printf("************************\n");
        print_list(free_list, "Free Memory");
        print_list(alloc_list, "Allocated Memory");
        printf("\n");
    }

    list_free(free_list);
    list_free(alloc_list);
    return 0;
}
