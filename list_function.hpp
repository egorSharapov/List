#ifndef LIST_FUNCTION_HPP_INCLUDED
#define LIST_FUNCTION_HPP_INCLUDED

#include <stdio.h>

#define LIST_OK(list)    unsigned long errors = list_check (list, SIMPLE);    \
                          if (errors != 0)                            \
                          {                                           \
                              print_list_error (errors);              \
                              list_dump (dump_file, list);            \
                              exit(errors);                           \
                          }

const size_t BASIC_LIST_SIZE = 10;
const int val_posion = -10000;
typedef int Elem_t;
const int multiple = 2;
const int cash_size = 10;

enum LIST_ERRORS
{
    CAPACITY_ERROR = 0,
    SIZE_MORE_THAN_CAPACITY_ERROR = 1,
    SIZE_ERROR = 2,
    FREE_ERROR = 3,
    NODE_NULL_PTR_ERROR    = 4,
    LIST_NULL_PTR_ERROR    = 5,
    TALE_ERROR = 6,
    HEAD_ERROR = 7,
};

enum CHECK_MODE
{
    SIMPLE = 0,
    FULL = 1,
};

typedef struct Node
{
    Elem_t val = 0;
    int next   = -1;
    int prev   = -1;
};


typedef struct List
{
    Node *nodes      = NULL;
    Node *cash_nodes = NULL;
    size_t  capacity = 0;
    size_t  size     = 0;
    unsigned tale    = 0;
    unsigned head    = 0;
    unsigned free    = 0;
    bool   is_linear = true;
};

void   list_ctor   (List *list, size_t list_size);
void   list_dtor   (List *list);
void   list_dump   (FILE *dump_file, List *list);
void   list_resize (List *list);
void   graph_list_dump (List *list);
void   list_sort   (List *list);
void   list_push   (List  *list, Elem_t value);
int    list_insert (List *list, Elem_t value, int insert_index);
Elem_t list_remove (List *list, int remove_index);
Elem_t list_pop    (List *list);
void   list_fit    (List *list);


#endif