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