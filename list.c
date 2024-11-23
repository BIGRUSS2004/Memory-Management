#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_t *list_alloc() {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    list->head = NULL;
    return list;
}

node_t *node_alloc(block_t *blk) {
    node_t *node = (node_t *)malloc(sizeof(node_t));
    node->blk = blk;
    node->next = NULL;
    return node;
}

void list_free(list_t *l) {
    node_t *current = l->head;
    while (current) {
        node_t *temp = current;
        current = current->next;
        free(temp->blk);
        free(temp);
    }
    free(l);
}

void list_add_to_front(list_t *l, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    new_node->next = l->head;
    l->head = new_node;
}

void list_add_to_back(list_t *l, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    if (!l->head) {
        l->head = new_node;
    } else {
        node_t *current = l->head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}

void list_add_ascending_by_address(list_t *l, block_t *newblk) {
    node_t *new_node = node_alloc(newblk);
    node_t *current = l->head;
    node_t *prev = NULL;

    while (current && newblk->start > current->blk->start) {
        prev = current;
        current = current->next;
    }

    if (!prev) {
        new_node->next = l->head;
        l->head = new_node;
    } else {
        prev->next = new_node;
        new_node->next = current;
    }
}

void list_add_ascending_by_blocksize(list_t *l, block_t *newblk) {
    node_t *new_node = node_alloc(newblk);
    node_t *current = l->head;
    node_t *prev = NULL;
    int new_size = newblk->end - newblk->start + 1;

    while (current) {
        int current_size = current->blk->end - current->blk->start + 1;
        if (new_size < current_size) {
            break;
        }
        prev = current;
        current = current->next;
    }

    if (!prev) {
        new_node->next = l->head;
        l->head = new_node;
    } else {
        prev->next = new_node;
        new_node->next = current;
    }
}

void list_add_descending_by_blocksize(list_t *l, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    node_t *current = l->head;
    node_t *prev = NULL;
    int new_size = blk->end - blk->start + 1;

    while (current) {
        int current_size = current->blk->end - current->blk->start + 1;
        if (new_size > current_size) {
            break;
        }
        prev = current;
        current = current->next;
    }

    if (!prev) {
        new_node->next = l->head;
        l->head = new_node;
    } else {
        prev->next = new_node;
        new_node->next = current;
    }
}

block_t *list_remove_from_front(list_t *l) {
    if (!l->head) {
        return NULL;
    }

    node_t *temp = l->head;
    block_t *blk = temp->blk;
    l->head = l->head->next;
    free(temp);
    return blk;
}

void list_coalese_nodes(list_t *l) {
    node_t *current = l->head;

    while (current && current->next) {
        block_t *curr_blk = current->blk;
        block_t *next_blk = current->next->blk;

        if (curr_blk->end + 1 == next_blk->start) {
            curr_blk->end = next_blk->end;
            node_t *temp = current->next;
            current->next = temp->next;
            free(temp->blk);
            free(temp);
        } else {
            current = current->next;
        }
    }
}
