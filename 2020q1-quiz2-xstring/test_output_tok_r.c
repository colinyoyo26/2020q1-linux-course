#include "string.h"
#include "xs.h"

int main(void)
{
    xs str1, str2;
    char token[8] = "@#$(^&*", *tem;
    char *saveptr1, *saveptr2;
    char out1[20], out2[20];
    char *out1ptr = out1, *out2ptr = out2;

    xs_new(&str1, "my@name&is(yoyi");
    xs_new(&str2, "foo@bar&bar(foooo");
    printf("#token: %s\n", token);
    printf("#initial str1: %s\n", xs_data(&str1));
    printf("#initial str2: %s\n", xs_data(&str2));

    for (xs *p1 = &str1, *p2 = &str2;; p1 = p2 = NULL) {
        char *tem1 = xs_tok_r(p1, token, &saveptr1);
        char *tem2 = xs_tok_r(p2, token, &saveptr2);
        if (!*tem1)
            break;
        strcpy(out1ptr, tem1);
        strcpy(out2ptr, tem2);
        out1ptr += strlen(tem1) + 1;
        out2ptr += strlen(tem2) + 1;
        *(out1ptr - 1) = *(out2ptr - 1) = ' ';
    }
    *(out1ptr - 1) = *(out2ptr - 1) = '\0';
    printf("out1: %s\n", out1);
    printf("out2: %s\n", out2);
    return 0;
}