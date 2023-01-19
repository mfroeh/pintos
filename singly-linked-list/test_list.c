#include "list.h"
#include <stdio.h>

int main(void) {
  list_item root = {-99, NULL};
  append(&root, 1);
  append(&root, 2);
  prepend(&root, 0);
  prepend(&root, -1);
  print(&root);
  clear(&root);
  print(&root);

  input_sorted(&root, -1);
  append(&root, 10);
  prepend(&root, -2);
  input_sorted(&root, 5);
  input_sorted(&root, 4);
  input_sorted(&root, 5);
  input_sorted(&root, 7);
  input_sorted(&root, 13);
  print(&root);
  clear(&root);
  print(&root);
  
  input_sorted(&root, 123);
  input_sorted(&root, -123);
  prepend(&root, 234); // This breaks the sort, which is the expected result
  input_sorted(&root, 235);
  print(&root);
  clear(&root);
}
