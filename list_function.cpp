#include "stdlib.h"
#include "list_function.hpp"
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include "utilities.hpp"

int dump_counter = 0;

void list_ctor (List *list, size_t list_size)
{
    if ((int) list_size < 0)
        list_size = BASIC_LIST_SIZE;

    list->nodes = (Node *) calloc (list_size, sizeof (Node));
      
    list->capacity = list_size;
    list->head = 1;
    list->tale = 1;
    list->size = 1;
    list->free = 1;
    
    list->nodes[0].val  = 0;
    list->nodes[0].next = 0;
    list->nodes[0].prev = 0;

    for (int index = list->head; index < list->capacity; index++)
    {
        list->nodes[index].val = val_posion;
        list->nodes[index].next = index + 1;
        list->nodes[index].prev = - 1;
    }
    list->nodes[list->capacity - 1].next = 0;
    list->nodes[list->capacity - 1].prev = - 1;
}

void list_dtor (List *list)
{
    assert (list);

    free (list->nodes);
    list->nodes = NULL;

    list->size = -1;
    list->capacity = -1;
    list->tale = -1;
    list->head = -1; 
    list->free = -1;
    list->is_linear = false;
    list = NULL;
}


void list_push (List  *list, Elem_t value)
{
    list_insert (list, value, list->tale);
}

Elem_t list_pop (List *list)
{
    Elem_t value = list_remove (list, list->tale);
    return value;
}


int list_insert (List *list, Elem_t value, int insert_index)
{
    assert (list);

    if (list->nodes[insert_index].prev == -1 and list->head != list->tale)
    {
        printf ("wrong index\n");
        return insert_index;
    }

    if (list->capacity == list->size)
        list_resize (list);

    if (insert_index != list->tale)
        list->is_linear = false;
    else
        list->tale = list->free;

    int new_free = list->nodes[list->free].next;
    

    list->nodes[list->free].val = value;

    if (insert_index != list->head or list->tale != list->head)
        list->nodes[list->free].prev = insert_index;
    else
        list->nodes[list->free].prev = 0;
    
    list->nodes[list->free].next = list->nodes[insert_index].next;
    list->nodes[insert_index].next = list->free;
    
    if (list->tale == insert_index)
        list->nodes[list->free].next = 0;

    list->free = new_free;
    list->size += 1;

    

    return insert_index;
}


Elem_t list_remove (List *list, int remove_index)
{
    Elem_t remove_val = val_posion;

    assert (list);

    int new_free = list->nodes[list->free].next;

    //___ связывание соседних элементов_____________
    int next_index = list->nodes[remove_index].next;
    int prev_index = list->nodes[remove_index].prev;

    if (prev_index != 0)
        list->nodes[prev_index].next = next_index;
    
    if (next_index != 0)
        list->nodes[next_index].prev = prev_index;

    //___заполнение зоны free___________________
    remove_val =  list->nodes[remove_index].val;
    list->nodes[remove_index].val = val_posion;
    list->nodes[remove_index].prev = -1;
    list->nodes[remove_index].next = list->free;
    list->free = remove_index;

    if (remove_index != list->tale)
        list->is_linear = false;

    if (remove_index == list->tale)
        list->tale = prev_index;
    
    list->size -= 1;

    return remove_val;
}



void list_dump (FILE *dump_file, List *list)
{
    assert (list);

    fprintf (dump_file, " size = %d\n capacity = %d\n", list->size, list->capacity);
    fprintf (dump_file, " is_linear = %d\n\n", list->is_linear);
    
    for (int index = 0; index < list->capacity; index++)
    {
        if (index == list->free)
            fprintf (dump_file, "|>free");
        else if  (index == list->head)
            fprintf (dump_file, "|>head");
        else if (index == list->tale)
            fprintf (dump_file, "|>tale");
        else
            fprintf (dump_file, "      ");
    }

    fprintf (dump_file, "\n|");

    for (int index = 0; index < list->capacity; index++)
    {
        fprintf (dump_file, " %03d |", index);
    }

    fprintf (dump_file, "\n|");
    
    for (int index = 0; index < list->capacity; index++)
    {
        if (list->nodes[index].val != val_posion)
            fprintf (dump_file, " %3d |", list->nodes[index].val);
        else
            fprintf (dump_file, "  P  |");
    }

    fprintf (dump_file, "\n|");

    for (int index = 0; index < list->capacity; index++)
    {
        fprintf (dump_file, "%2d|%2d|", list->nodes[index].prev, list->nodes[index].next);
    }
    fprintf (dump_file, "\n\n");
}


void graph_list_dump (List *list)
{
    FILE *graph_file = fopen ("out\\input.dot", "wb");

    fprintf (graph_file, "digraph {\n    node [shape=Mrecord, width = 0.5];\n    rankdir=LR;\n");
    fprintf (graph_file, "    free [shape = Mrecord];\n");
    for (int index = 0; index < list->capacity; index++)
    {
        fprintf (graph_file, "    struct%d [shape= Mrecord, label = \"%d|", index, index);
        
        if (list->nodes[index].val != val_posion)
            fprintf (graph_file, "%d", list->nodes[index].val);
        else 
            fprintf (graph_file, "P");
        
        fprintf (graph_file,"|{%d|%d}\"];\n", list->nodes[index].prev, list->nodes[index].next);
    }

    fprintf (graph_file, "\n    ");

    fprintf (graph_file, "edge [dir = both, color = white]\n    ");

    for (int index = 0; index < list->capacity - 1; index++)
    {
        fprintf (graph_file, "struct%d ->", index);
    }
    fprintf (graph_file, "struct%d;\n    ", list->capacity - 1);

    fprintf (graph_file, "edge [dir = both, color = blue]\n    ");
    int next = list->head;

    for (int index = 0; index < list->size - 2; index++)
    {
        fprintf (graph_file, "struct%d ->", next);
        next = list->nodes[next].next;
    }
    fprintf (graph_file, "struct%d;\n    ", list->tale);


    fprintf (graph_file, "edge [dir = right, color = gold]\n");

    next = list->free;

    for (int index = 0; index < list->capacity - list->size; index++)
    {
        fprintf (graph_file, "struct%d ->", next);
        next = list->nodes[next].next;
    }
    fprintf (graph_file, "struct%d;\n}", next);

    fclose (graph_file);


    dump_counter += 1;
    system ("dot -Tsvg out\\input.dot > out\\output%d.svg");

}

// char *make_command (int dump_counter)
// {
//     char *origin = "dot -Tsvg out\\input.dot > out\\output";
//     char *buffer = (char *) calloc (strlen (origin)*2, sizeof (char));

//     buffer = strcpy( buffer, origin);
//     boffer = strcat (buffer,);
// }

void list_resize (List *list)
{
    assert (list);

    size_t old_capacity = list->capacity;

    list->capacity *= multiple;

    list->nodes = (Node *) recalloc (list->nodes, list->capacity, sizeof (list->nodes[0]));
    
    if (list->nodes == NULL)
        printf ("calloc error");

    for (int index = old_capacity; index < list->capacity; index++)
    {
        list->nodes[index].val = val_posion;
        list->nodes[index].next = index + 1;
        list->nodes[index].prev = -1;
    }
    list->nodes[list->capacity - 1].next = list->free;
    list->free = old_capacity;
}

void list_fit (List *list)
{
    list_sort (list);
    
    list->capacity = list->size;
    list->nodes = (Node *) recalloc (list->nodes, list->capacity, sizeof(list->nodes[0]));
}

void list_sort (List *list)
{
    Node *new_nodes = (Node *) calloc (list->capacity, sizeof(list->nodes[0]));

    int next = list->head;
    list->head = 1;

    new_nodes[0].val = 0;
    new_nodes[0].next = 0;
    new_nodes[0].prev = 0;

    for (int index = 1; index < list->size; index++)
    {
        new_nodes[index].val = list->nodes[next].val;
        new_nodes[index].next = index + 1;
        new_nodes[index].prev = index - 1;
        next = list->nodes[next].next;
    }

    new_nodes[list->size - 1].next = 0;

    for (int index = list->size; index < list->capacity; index++)
    {
        new_nodes[index].val = val_posion;
        new_nodes[index].next = index + 1;
        new_nodes[index].prev = -1;
    }

    new_nodes[list->capacity - 1].next = 0;
    
    list->tale = list->size - 1;
    list->free = list->size;
    list->is_linear = true;
    
    list->nodes = new_nodes;
}



// void create_cash (List *list, int cash_size, int begin_index)
// {
//     list->cash_nodes = (Node*) calloc (cash_size, sizeof (list->nodes[0]));

//     if (list->cash_nodes == NULL)
//         printf ("calloc list cash error");


//     for (int index = 0; index < cash_size; index++)
//     {
//         list
//     }
    
// }