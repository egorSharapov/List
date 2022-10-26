#ifndef LIST_FUNCTION_HPP_INCLUDED
#define LIST_FUNCTION_HPP_INCLUDED

#include <stdio.h>
#include "list.hpp"


const size_t BASIC_LIST_SIZE = 10;

const int multiple = 2;
const int cash_size = 10;


void   list_ctor   (List *list, size_t list_size);
void   list_dtor   (List *list);
void   list_resize (List *list);
void   list_sort   (List *list);
void   list_push   (List  *list, Elem_t value);
size_t list_insert_after  (List *list, Elem_t value, size_t insert_index);
size_t list_insert_before (List *list, Elem_t value, size_t insert_index);
size_t list_translate_logical_index_to_physical_position_dont_call_this_function (List *list, size_t logical_index);
size_t list_translate_physical_position_to_logical_adress_dont_call_this_function (List *list, size_t physical_position);
Elem_t list_remove (List *list, size_t remove_index);
Elem_t list_pop    (List *list);
void   list_fit    (List *list);

size_t list_free_mem (List *list);
void   list_fill_free (List *list, size_t begin, size_t end);

#endif