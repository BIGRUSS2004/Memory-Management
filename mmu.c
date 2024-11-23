#include "mmu.h"
#include <stdio.h>
#include <stdlib.h>

void allocate_memory(list_t *freelist, list_t *alloclist, int pid, int blocksize, int policy) {
    node_t *current = freelist->head, *prev = NULL;
    block_t *fragment;

    while (current) {
        int current_size = current->blk->end - current->blk->start + 1;
        if (current_size >= blocksize) {
            block_t *allocated = current->blk;
            allocated->pid = pid;
            allocated->end = allocated->start + blocksize - 1;

            if (prev) prev->next = current->next;
            else freelist->head = current->next;

            list_add_ascending_by_address(alloclist, allocated);

            if (allocated->end < current->blk->end) {
                fragment = malloc(sizeof(block_t));
                fragment->start = allocated->end + 1;
                fragment->end = current->blk->end;
                fragment->pid = 0;
                if (policy == 1) list_add_to_back(freelist, fragment);
                else if (policy == 2) list_add_ascending_by_blocksize(freelist, fragment);
                else list_add_descending_by_blocksize(freelist, fragment);
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Error: Memory Allocation %d blocks\n", blocksize);
}

void deallocate_memory(list_t *alloclist, list_t *freelist, int pid, int policy) {
    node_t *current = alloclist->head, *prev = NULL;

    while (current) {
        if (current->blk->pid == pid) {
            block_t *deallocated = current->blk;
            deallocated->pid = 0;

            if (prev) prev->next = current->next;
            else alloclist->head = current->next;

            if (policy == 1) list_add_to_back(freelist, deallocated);
            else if (policy == 2) list_add_ascending_by_blocksize(freelist, deallocated);
            else list_add_descending_by_blocksize(freelist, deallocated);

            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Error: Can't locate Memory Used by PID: %d\n", pid);
}

list_t* coalese_memory(list_t *list) {
    list_t *temp_list = list_alloc();
    block_t *blk;
    while ((blk = list_remove_from_front(list)) != NULL)
        list_add_ascending_by_address(temp_list, blk);
    list_coalese_nodes(temp_list);
    return temp_list;
}

void print_list(list_t *list, char *message) {
    node_t *current = list->head;
    printf("%s:\n", message);
    while (current) {
        block_t *blk = current->blk;
        printf("START: %d END: %d PID: %d\n", blk->start, blk->end, blk->pid);
        current = current->next;
    }
}
