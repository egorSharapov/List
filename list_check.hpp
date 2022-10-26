#ifndef LIST_CHECK_HPP_INCLUDED
#define LIST_CHECK_HPP_INCLUDED

#include "list.hpp"

#define LIST_OK(list)    unsigned long errors = list_check (list, SIMPLE);    \
                          if (errors != 0)                            \
                          {                                           \
                                           \
                              list_dump (dump_file, list);            \
                              exit(errors);                           \
                          }

#define list_dump(output_file, list) \
                          list_dump_(output_file, list, __FUNCTION__, __FILE__, __LINE__)



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

unsigned long list_check (List *list, CHECK_MODE mode);

bool   list_check_data (List *list);
bool   list_check_free (List *list);
void   list_dump_ (FILE *file, List *list, const char *func_name, const char *file_name, const int line);
void   graph_list_dump (List *list);
void   print_list_error (FILE* dump_file, unsigned long errors_code);

void build_png_to_html ();

#endif