#include "list_function.hpp"
#include "list_check.hpp"
#include "utilities.hpp"
#include <time.h>
#include <stdlib.h>

const char *dump_file_name = "out\\list_dump_file.txt";
FILE *dump_file = open_file (dump_file_name, "wb");;

int main ()
{
    time_t current_time = 0;
    srand((unsigned) time (&current_time));

    List list = {};

    list_ctor (&list, 4);

    graph_list_dump (&list);

    for (int i = 1; i < 20; i++)
    {
        list_dump (dump_file, &list);
        list_insert_after (&list, i*10 + 2, rand() % list.tale + 1);
    }
    list_remove (&list, 5);
    graph_list_dump (&list);

    printf ("physical position = %u\n", list_translate_logical_index_to_physical_position_dont_call_this_function  (&list, 5));
    printf ("logical adress = %u\n",    list_translate_physical_position_to_logical_adress_dont_call_this_function (&list, 3));
    printf ("free mem = %u\n", list_free_mem (&list));
    printf ("el index = %u\n", list_find_value (&list, 102));  
    
    graph_list_dump (&list);
    list_dump (dump_file, &list);

    list_sort (&list);
    list_dump (dump_file, &list);
    
    list_insert_after (&list, 300, list.tale);  
    list_dump (dump_file, &list);
    graph_list_dump (&list);


    list.nodes[10].prev = -1;
    
    print_list_error (dump_file, list_check (&list, FULL));

    list_dump (dump_file, &list);

    list_clear (&list);
    list_dump (dump_file, &list);
    
 
    list_dtor (&list);

    print_list_error (dump_file, list_check (&list, FULL));

    build_png_to_html ();

    return 0;
}