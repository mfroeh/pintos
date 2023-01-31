#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct student {
    char *name;
    struct list_elem elem;
};

typedef struct student student;

// Reads characters from stdio and allocates a new c-string for it
char* read_name() {
    char buf[1024];
    size_t bytes = read(0, &buf, 1024);

    char* name = malloc(bytes);
    for (size_t i = 0; i < bytes-1; ++i) {
        name[i] = buf[i];
    }
    // The read characters include the line feed character \n which we replace with '\0' to terminate the string
    name[bytes-1] = '\0';
    return name;
}

// Fetch a student name from the terminal input and add it to the list
void insert (struct list *student_list) {
    char* name = read_name();
    student* stu = malloc(sizeof(student));
    stu->name = name;
    list_push_back(student_list, &stu->elem);
}

// Get a student name from the terminal input, remove it from the list, and deallocat ethe appropriate memory (if it exists in the list)
void delete (struct list *student_list) {
    char* name = read_name();
    struct list_elem *it;
    for (it = list_begin(student_list); it != list_end(student_list); it = list_next(it)) {
        student* stu = list_entry(it, struct student, elem);
        if (strcmp(stu->name, name) == 0) {
            list_remove(it);
            printf("Removed: %s\n", stu->name);
            free(stu->name);
            free(stu);
            break;
        }
    }
    free(name);
}

// Print the entire list
void list (struct list *student_list) {
    struct list_elem *it;
    for (it = list_begin(student_list); it != list_end(student_list); it = list_next(it)) {
        student* stu = list_entry(it, struct student, elem);
        printf("%s ", stu->name);
    }
    printf("\n");
}

// Clear the list and deallocate the memory for all items in the list then exit(0)
void quit (struct list *student_list) {
    while (!list_empty(student_list)) {
        struct list_elem *e = list_pop_front(student_list);
        student *stu = list_entry(e, struct student, elem);
        free(stu->name);
        free(stu);
    }
    exit(0);
}


int main() {
    struct list student_list;
    list_init (&student_list);
    int opt;

    do {
        printf("Menu:\n");
        printf("1 - Insert student\n");
        printf("2 - Delete student\n");
        printf("3 - List students\n");
        printf("4 - Exit\n");
        scanf("%d", &opt);
        switch (opt) {
            case 1:
                {
                    insert(&student_list);
                    break;
                }
            case 2:
                {
                    delete(&student_list);
                    break;
                }
            case 3:
                {
                    list(&student_list);
                    break;
                }

            case 4:
                {
                    quit(&student_list);
                    break;
                }
            default:
                {
                    printf("Quit? (1/0):\n");
                    scanf("%d", &opt);
                    if (opt)
                        quit(&student_list);
                    break;
                }
        }
    } while(1);

    return 0;
}
