#include "stdlib.h"
#include "list_function.hpp"
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include "utilities.hpp"
#include "list_check.hpp"


extern FILE *dump_file;

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

    list_fill_free (list, list->head, list->capacity - 1);

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

size_t list_translate_logical_index_to_physical_position_dont_call_this_function (List *list, size_t logical_index)
{
    assert (list);

    size_t physical_position = list->head;

    for (size_t index = 1; index < logical_index; index++)
    {
        physical_position = list->nodes[physical_position].next;
    }

    return physical_position;
}

size_t list_translate_physical_position_to_logical_adress_dont_call_this_function (List *list, size_t physical_position)
{
    assert (list);

    size_t logical_position = 1;

    if (list->nodes[physical_position].prev == -1 or physical_position >= list->capacity)
        return -1;

    size_t index = physical_position;

    while (index != list->head)
    {
        index = list->nodes[index].prev;
        logical_position += 1;
    }
    return logical_position;
}

void list_push (List  *list, Elem_t value)
{
    list_insert_after (list, value, list->tale);
}

Elem_t list_pop (List *list)
{
    Elem_t value = list_remove (list, list->tale);
    return value;
}


size_t list_insert_before (List *list, Elem_t value, size_t insert_index)
{
    LIST_OK (list);

    if (list->nodes[insert_index].prev == -1 and list->head != list->tale)
    {
        printf ("wrong index\n");
        return insert_index;
    }

    if (list->capacity == list->size)
        list_resize (list);

    list->is_linear = false;
    list->head = list->free;

    size_t new_free = list->nodes[list->free].next;
    

    list->nodes[list->free].val = value;

    if (insert_index != list->tale or list->tale != list->head)
        list->nodes[list->free].next = insert_index;
    else
        list->nodes[list->free].next = 0;
    
    //list->nodes[list->nodes[insert_index].prev].next = list->free;

    list->nodes[list->free].prev = list->nodes[insert_index].prev;
    list->nodes[insert_index].prev = list->free;
    
    if (list->head == insert_index)
        list->nodes[list->free].prev = 0;

    list->free = new_free;
    list->size += 1;

    

    return insert_index;
}

size_t list_insert_after (List *list, Elem_t value, size_t insert_index)
{
    LIST_OK (list);

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

    size_t new_free = list->nodes[list->free].next;
    

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


Elem_t list_remove (List *list, size_t remove_index)
{
    LIST_OK (list);

    if (list->nodes[remove_index].prev == -1)
        printf ("ERROR remove free zone\n");

    Elem_t remove_val = val_posion;


    //size_t new_free = list->nodes[list->free].next;

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

void list_resize (List *list)
{
    assert (list);

    size_t old_capacity = list->capacity;

    list->capacity *= multiple;

    list->nodes = (Node *) recalloc (list->nodes, list->capacity, sizeof (list->nodes[0]));
    
    if (list->nodes == NULL)
        printf ("calloc error");

    for (size_t index = old_capacity; index < list->capacity; index++)
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
    LIST_OK (list);

    list_sort (list);
    
    list->capacity = list->size;
    list->nodes = (Node *) recalloc (list->nodes, list->capacity, sizeof(list->nodes[0]));
}

void list_sort (List *list)
{
    LIST_OK (list);

    Node *new_nodes = (Node *) calloc (list->capacity, sizeof(list->nodes[0]));

    int next = list->head;
    list->head = 1;

    new_nodes[0].val = 0;
    new_nodes[0].next = 0;
    new_nodes[0].prev = 0;

    for (size_t index = 1; index < list->size; index++)
    {
        new_nodes[index].val = list->nodes[next].val;
        new_nodes[index].next = index + 1;
        new_nodes[index].prev = index - 1;
        next = list->nodes[next].next;
    }

    new_nodes[list->size - 1].next = 0;

    for (size_t index = list->size; index < list->capacity; index++)
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


size_t list_free_mem (List *list)
{
    assert (list);

    size_t free_mem = 0;
    size_t next = list->free;

    while ((next = list->nodes[next].next) != 0)
        free_mem += 1;
    free_mem += 1;

    return free_mem;
}


void list_fill_free (List *list, size_t begin, size_t end)
{
    for (size_t index = begin; index < end + 1; index++)
    {
        list->nodes[index].val = val_posion;
        list->nodes[index].next = index + 1;
        list->nodes[index].prev = - 1;
    }
    list->nodes[end].next = 0;
    list->nodes[end].prev = - 1;
}