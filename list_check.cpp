#include "list_check.hpp"
#include <windows.h>
#include <assert.h>
#include "utilities.hpp"

int dump_counter = 0;

unsigned long list_check (List *list, CHECK_MODE mode)
{
    unsigned long error = 0;
    if (list == NULL)
    {
        error = error | (1 << LIST_NULL_PTR_ERROR);
        return error;
    }
    if  ((int) list->size > (int) list->capacity)
        error = error | (1 << SIZE_MORE_THAN_CAPACITY_ERROR);
    
    if (list->nodes == NULL)
        error = error | (1 << NODE_NULL_PTR_ERROR);
    
    if (((int) list->capacity < 0))
        error = error | (1 << CAPACITY_ERROR);

    if ((int) list->head < 0)
    {
        error = error | (1 << HEAD_ERROR);
    }
    if ((int) list->tale < 0)
        error = error | (1 << TALE_ERROR);

    if ((int) list->free < 0)
        error = error | (1 << FREE_ERROR); 

    if (mode == FULL and error == 0)
    {
        if (list_check_data (list))
            error = error | (1 << DATA_ERROR);

        if (list_check_free (list))
            error = error | (1 << FREE_ERROR);
    }
    return error;
}


bool list_check_data (List *list)
{
    int next = list->head; 
    int prev = 0;

    for (size_t index = 0; index < list->size - 1; index++)
    {
        if ((int) list->nodes[next].next < 0)
            return 1;
        if (list->nodes[next].prev != prev)
            return 1;
        
        prev = next;
        next = list->nodes[next].next;
    }
    return 0;
}

bool list_check_free (List *list)
{
    int next = list->free;
    for (size_t index = 0; index < list->capacity - list->size; index++)
    {
        if (list->nodes[next].prev != -1)
            return 1;
        
        next = list->nodes[next].next;
    }
    return 0;
}


void print_list_error (FILE* dump_file, unsigned long errors_code)
{
    if (errors_code >> SIZE_MORE_THAN_CAPACITY_ERROR & 1)
        fprintf (dump_file, "SIZE_MORE_THAN_CAPACITY_ERRO\n");
    
    if (errors_code >> LIST_NULL_PTR_ERROR & 1)
        fprintf (dump_file, "LIST_NULL_PTR_ERROR\n");
    
    if (errors_code >> CAPACITY_ERROR & 1)
        fprintf (dump_file, "CAPACITY ERROR\n");

    if (errors_code >> HEAD_ERROR & 1)
        fprintf (dump_file, "HEAD_ERROR\n");

    if (errors_code >> TALE_ERROR & 1)
        fprintf (dump_file, "TALE_ERROR\n");
    
    if (errors_code >> FREE_ERROR & 1)
        fprintf (dump_file, "FREE_ERROR\n");
    
    if (errors_code >> DATA_ERROR & 1)
        fprintf (dump_file, "DATA_ERROR\n");
    
    // if (errors_code >> LEFT_CANARY_ERROR & 1)
    //     fprintf (output_file, "LEFT CANARY ERROR\n");
    // if (errors_code >> RIGHT_CANARY_ERROR & 1)
    //     fprintf (output_file, "RIGHT CANARY ERROR\n");
    
    // if (errors_code >> LEFT_DATA_CANARY_ERROR & 1)
    //     fprintf (output_file, "LEFT DATA CANARY ERROR\n");
    
    // if (errors_code >> RIGHT_DATA_CANARY_ERROR & 1)
    //     fprintf (output_file, "RIGHT DATA CANARY ERROR\n");

    // #ifdef HASHCHECK
    // if (errors_code >> HASH_ERROR & 1)
    //     fprintf (output_file, "HASH ERROR\n");
    // #endif 
}


void list_dump_ (FILE *dump_file, List *list, const char *func_name, const char *file_name, const int line)
{
    assert (list);
    assert (dump_file);
    
    fprintf (dump_file, "\n%s() at %s.cpp (%d)\n", func_name, file_name, line);
    fprintf (dump_file, " size = %d\n capacity = %d\n", list->size, list->capacity);
    fprintf (dump_file, " is_linear = %d\n\n", list->is_linear);
    
    for (size_t index = 0; index < list->capacity; index++)
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

    for (size_t index = 0; index < list->capacity; index++)
    {
        fprintf (dump_file, " %03d |", index);
    }

    fprintf (dump_file, "\n|");
    
    for (size_t index = 0; index < list->capacity; index++)
    {
        if (list->nodes[index].val != val_posion)
            fprintf (dump_file, " %3d |", list->nodes[index].val);
        else
            fprintf (dump_file, "  P  |");
    }

    fprintf (dump_file, "\n|");

    for (size_t index = 0; index < list->capacity; index++)
    {
        fprintf (dump_file, "%2d|%2d|", list->nodes[index].prev, list->nodes[index].next);
    }
    fprintf (dump_file, "\n\n");
}


void graph_list_dump_  (List *list, const char *func_name, const char *file_name, const int line)
{

    FILE *graph_file = fopen ("out\\input.dot", "w");
    if (graph_file == NULL)
        printf ("open %s error", "out\\input.dot");

    fprintf (graph_file, "digraph {\n");
    fprintf (graph_file, "    labelloc = \"t\"\n");
    fprintf (graph_file, "    fontsize = \"40\"\n");
    fprintf (graph_file, "    label = \"%s() at %s.cpp (%d)\"\n", func_name, file_name, line);
    fprintf (graph_file, "    splines=ortho\n    rankdir=LR\n");

    for (size_t index = 0; index < list->capacity; index++)
    {
        fprintf (graph_file, "    struct%d [shape= Mrecord, label = \"%d|", index, index);
        
        if (list->nodes[index].val != val_posion)
            fprintf (graph_file, "%d", list->nodes[index].val);
        else 
            fprintf (graph_file, "P");
        
        fprintf (graph_file,"|{%d|%d}\"]\n", list->nodes[index].prev, list->nodes[index].next);
    }
    fprintf (graph_file, "\n    ");

    for (size_t index = 0; index < list->capacity - 1; index++)
    {
        fprintf (graph_file, "struct%d -> struct%d [style = \"invis\", weight = 1000]\n    ", index, index + 1);
    }
    
    fprintf (graph_file, "\n    ");

    fprintf (graph_file, "free->struct%d {rank = same; free; struct%d;}\n    ", list->free, list->free);


    fprintf (graph_file, "edge [dir = both, color = blue]\n    ");

    size_t next = list->head;

    if (list->size > 1)
    {
        for (size_t index = 0; index < list->size - 2; index++)
        {
            fprintf (graph_file, "struct%d -> struct%d [splines = ortho]\n    ", next, list->nodes[next].next);
            next = list->nodes[next].next;
        }
        fprintf (graph_file, "\n    ");
    }

    fprintf (graph_file, "edge [dir = right, color = gold]\n    ");

    next = list->free;

    for (size_t index = 0; index < list->capacity - list->size - 1; index++)
    {
        fprintf (graph_file, "struct%d ->", next);
        next = list->nodes[next].next;
    }
    fprintf (graph_file, "struct%d\n    ", next);

    fprintf (graph_file, "struct%d -> struct0 [color = black]\n    ", next);
    fprintf (graph_file, "struct0 -> struct0 [dir = both, color = black]}\n    ");

    fclose (graph_file);

    dump_counter += 1;

    char cmd_string[max_string_size] = "";

    sprintf (cmd_string, "dot out\\input.dot -Tpng -o out\\output%d.png", dump_counter);
    system (cmd_string);
}


void build_png_to_html ()
{
    FILE* file = open_file ("out\\output.html", "wb");

    fprintf (file, " <pre>\n\n");
    
    for (int index = 1; index < dump_counter + 1; index++)
    {
        fprintf (file, "<image src = \"output%d.png\" /image>\n", index);
    }

    fclose (file);
}