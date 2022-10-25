#include "list_function.hpp"

const char *dump_file_name = "out\\list_dump.txt";

int main ()
{
    List list = {};

    FILE *dump_file = open_file (dump_file_name, "wb");
    list_ctor (&list, 9);

    for (int i = 1; i < 10; i++)
    {
        list_dump (dump_file, &list);
        list_insert (&list, i*10, list.tale);
    }

    list_dump (dump_file, &list);
    graph_list_dump (&list);
    list_remove (&list, 6);
    list_dump (dump_file, &list);
    list_remove (&list, 5);
    list_dump (dump_file, &list);
    list_insert (&list, 300, 4);
    list_dump (dump_file, &list);
    list_sort (&list);
    list_dump (dump_file, &list);
    list_fit (&list);
    list_dump (dump_file, &list);
    list_dtor (&list);
    return 0;

}