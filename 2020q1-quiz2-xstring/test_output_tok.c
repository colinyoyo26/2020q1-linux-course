#include "xs.h"

int main(void)
{
    xs string;
    char token[8] = "HELLO W", *tem;
    xs_new(&string, "HELLOW fooHELLOWbarHbarfooLbarLbarObarW");
    printf("#token\n%s\n", token);
    printf("\n#initial string\n%s\n\n", xs_data(&string));

    printf("%s\n", xs_tok(&string, token));
    while (tem = xs_tok(NULL, token))
        printf("%s\n", tem);
    return 0;
}