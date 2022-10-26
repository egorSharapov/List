#include "utilities.hpp"
#include <string.h>
#include <malloc.h>

FILE *open_file (const char *file_name ,const char *mode)
{
    FILE *file = fopen (file_name, mode);

    if (file == NULL)
        printf ("open file error %s", file_name);

    return file;
}


void *recalloc(void *ptr, size_t num, size_t size)
{
    size_t temp_size = _msize(ptr);
    
    ptr = realloc (ptr, size*num);

    if (!ptr and (num*size > temp_size))
        memset ((char *) ptr + temp_size, 0, num*size - temp_size);

    return ptr;
}