#ifndef LIST_H
#define LIST_H

struct list_item {
    int value;
    struct list_item *next;
};

typedef struct list_item list_item;

list_item* make_list_item(int x, list_item* next);

// Puts x at the end of the list
void append(list_item *first, int x);

// Puts x at the beginning of the list
void prepend(list_item *first, int x);

// Prints all elements in the list 
void print(list_item *first);

// Find the first element in the list larger than x and input x right before that element
void input_sorted(list_item *first, int x);

// Free everything dynamically allocated
void clear(list_item *first);

#endif
