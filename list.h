#ifndef LIST_H
#define LIST_H

typedef struct block {
    int pid;
    int start;
    int end;
} block_t;

typedef struct node {
    block_t *blk;
    struct node *next;
} node_t;

typedef struct list {
    node_t *head;
} list_t;

list_t *list_alloc();
node_t *node_alloc(block_t *blk);
void list_free(list_t *l);

void list_add_to_front(list_t *l, block_t *blk);
void list_add_to_back(list_t *l, block_t *blk);
void list_add_ascending_by_address(list_t *l, block_t *blk);
void list_add_ascending_by_blocksize(list_t *l, block_t *blk);
void list_add_descending_by_blocksize(list_t *l, block_t *blk);
block_t *list_remove_from_front(list_t *l);
void list_coalese_nodes(list_t *l);

#endif // LIST_H
