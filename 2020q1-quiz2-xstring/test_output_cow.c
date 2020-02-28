#include <stdio.h>
#include "xs.h"

xs string, cow1, cow2;
void print_info(char* str)
{   printf("\n%s\n", str);

    printf("\n#string");
    printf("\nstring: %s", xs_data(&string));
    printf("\n&string: %p", xs_data(&string));
    printf("\nreference count: %d\n", string.ref_cnt);

    printf("\n#cow1");
    printf("\nstring: %s", xs_data(&cow1));
    printf("\n&string: %p", xs_data(&cow1));
    printf("\nreference count: %d\n", cow1.ref_cnt);

    printf("\n#cow2");
    printf("\nstring: %s", xs_data(&cow2));
    printf("\n&string: %p", xs_data(&cow2));
    printf("\nreference count: %d\n", cow2.ref_cnt);
}

int main()
{
    
    xs_new(&string, "gggfoobarbarbarbarbarzzz");
    xs_cpy(&cow1, &string);
    xs_cpy(&cow2, &string);
    print_info("after cpy from string to cow1 & cow2");
    xs prefix = *xs_tmp("((("), suffix = *xs_tmp(")))");
    xs_concat(&cow1, &prefix, &suffix);
    print_info("after concat cow1");
    xs_cpy(&cow2, &cow1);
    print_info("after cpy from cow1 to cow2");
    xs_trim(&cow1, ")(zg");
    print_info("after trim cow1");

    return 0;
}