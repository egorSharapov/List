#include "list_function.hpp"
#include "list_check.hpp"
#include "utilities.hpp"
  
const char *dump_file_name = "out\\list_dump_file.txt";
FILE *dump_file = open_file (dump_file_name, "wb");;

int main ()
{
    List list = {};

    list_ctor (&list, 4);

    for (int i = 1; i < 10; i++)
    {
        list_dump (dump_file, &list);
        list_insert_after (&list, i*10 + 2, list.tale);
    }
    list_insert_before (&list, 300, list.head);
    list_remove (&list, 5);
    graph_list_dump (&list);

    printf ("physical position = %u\n", list_translate_logical_index_to_physical_position_dont_call_this_function  (&list, 5));
    printf ("logical adress = %u\n",    list_translate_physical_position_to_logical_adress_dont_call_this_function (&list, 3));
    printf ("free mem = %u\n", list_free_mem (&list));
    printf ("el index = %u\n", list_find_value (&list, 300));  
    graph_list_dump (&list);

    list_dump (dump_file, &list);
    list_sort (&list);
    list_dump (dump_file, &list);
    graph_list_dump (&list);
    list_clear (&list);
    list_dump (dump_file, &list);
    list_dtor (&list);

    build_png_to_html ();

    return 0;
}