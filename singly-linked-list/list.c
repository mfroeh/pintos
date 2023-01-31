#include "list.h"
#include <stdlib.h>
#include <stdio.h>

list_item* make_list_item(int x, list_item* next) {
    list_item *new = malloc(sizeof(list_item));
    new->next = next;
    new->value = x;
    return new;
}

// Puts x at the end of the list
void append(list_item *first, int x) {
    while (first->next != NULL) {
        first = first->next;
    }
    first->next = make_list_item(x, NULL);
}

// Puts x at the beginning of the list
void prepend(list_item *first, int x) {
    first->next = make_list_item(x, first->next);
}

// Prints all elements in the list 
void print(list_item *first) {
    first = first->next;
    while (first != NULL) {
        printf("%d ", first->value);
        first = first->next;
    }
    printf("\n");
}

// Find the first element in the list larger than x and input x right before that element
void input_sorted(list_item *first, int x) {
    // While we didn't reach the end and the next node is <= our value
    while (first->next != NULL && first->next->value <= x) {
        first = first->next;
    }
    first->next = make_list_item(x, first->next);
}

// Free everything dynamically allocated
void clear(list_item *first) {
    // Skip the first node since we didn't dynamically allocate it
    list_item *node = first->next;
    while (node != NULL) {
        list_item* next = node->next;
        free(node);
        node = next;
    }
    // Make sure that first is not pointing to freed memory
    first->next = NULL;
}
