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

char    *extract_line(char *stash)
{
    char *line;
    int len;
    int i;

    if(!stash || !stash[0])
        return NULL;
    len = line_len(stash);
    line = (char *)malloc(sizeof(char) * (len + 1));
    if(!line)
        return NULL;
    i = 0;
    // sadece daha önce bulunan uzunluk kadar kopyalıcam
    // içeride newlline gördün mü dur gibi hiçbir if konstorlü olmayaca
    while(i < len)
    {
        line[i] = stash[i];
        i++;
    }
    line[i] = '\0';
    return line;
}
