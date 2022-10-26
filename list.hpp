#ifndef LIST_HPP_INCLUDED
#define LIST_HPP_INCLUDED

#include "stdio.h"

const int val_posion = -10000;

typedef int Elem_t;

struct info
{
    const char *func_name  = NULL;
    const char *file_name  = NULL;
    const char *list_name  = NULL;
    int         line       = 0;
};

typedef struct 
{
    Elem_t val = 0;
    int next   = -1;
    int prev   = -1;
} Node;


typedef struct
{
    Node *nodes      = NULL;
    Node *cash_nodes = NULL;

    info   list_info = {};
    size_t  capacity = 0;
    size_t  size     = 0;

    size_t tale      = 0;
    size_t head      = 0;
    size_t free      = 0;

    bool   is_linear = true;
} List;



#endif
