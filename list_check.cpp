#include "list_check.hpp"

unsigned long list_check (List *list)
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
        error = error | (1 << HEAD_ERROR);
    
    if ((int) list->tale < 0)
        error = error | (1 << TALE_ERROR);

    if ((int) list->free < 0)
        error = error | (1 << FREE_ERROR); 
    
    return error;
}



void print_list_error (unsigned long errors_code)
{
    if (errors_code >> SIZE_MORE_THAN_CAPACITY_ERROR & 1)
        fprintf (dump_file, "SIZE_MORE_THAN_CAPACITY_ERRO\n");
    
    if (errors_code >> LIST_NULL_PTR_ERROR & 1)
        fprintf (dump_file, "LIST_NULL_PTR_ERROR\n");
    
    if (errors_code >> CAPACITY_ERROR & 1)
        fprintf (dump_file, "CAPACITY ERROR\n");
    
    // if (errors_code >> LEFT_CANARY_ERROR & 1)
    //     fprintf (output_file, "LEFT CANARY ERROR\n");
    // if (errors_code >> RIGHT_CANARY_ERROR & 1)
    //     fprintf (output_file, "RIGHT CANARY ERROR\n");
    
    // if (errors_code >> LEFT_DATA_CANARY_ERROR & 1)
    //     fprintf (output_file, "LEFT DATA CANARY ERROR\n");
    
    // if (errors_code >> RIGHT_DATA_CANARY_ERROR & 1)
    //     fprintf (output_file, "RIGHT DATA CANARY ERROR\n");

    if (errors_code >> LIST_NULL_PTR_ERROR & 1)
    {
        fprintf (dump_file, "LIST_NULL_PTR_ERROR\n");
        //exit (errors_code);
    }

    // #ifdef HASHCHECK
    // if (errors_code >> HASH_ERROR & 1)
    //     fprintf (output_file, "HASH ERROR\n");
    // #endif 
}