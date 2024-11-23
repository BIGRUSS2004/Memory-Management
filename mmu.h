#ifndef MMU_H
#define MMU_H

#include "list.h"

// Memory management functions
void allocate_memory(list_t *freelist, list_t *alloclist, int pid, int blocksize, int policy);
void deallocate_memory(list_t *alloclist, list_t *freelist, int pid, int policy);
list_t* coalese_memory(list_t *list);
void print_list(list_t *list, char *message);

#endif // MMU_H
