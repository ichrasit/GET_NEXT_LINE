#include "get_next_line.h"

int line_len(char *str)
{
    int i;
    int is_nl;

    i = 0;
    is_nl = 0;
    if(!str)
        return (0);
    while(str[i] && !is_nl)
    {
        // karakter '\n' ise isnl 1 olur değilse zaten 0 kalıcak
        is_nl = !(str[i] ^ 10);
        i++;
    }
    return i;
}