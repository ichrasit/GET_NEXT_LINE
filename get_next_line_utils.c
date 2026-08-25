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

char    *clean_stash(char *stash)
{
    int i;
    int j;
    char    *new_stash;

    // branchless fonksiyonumuzla satırın nerede bittiğini bulucam burda
    i = line_len(stash);
    // eğer satır bittikten sonra okunacak bir şey kalmamışsa stash temizle null don

    if(!stash[i])
    {
        free(stash);
        return NULL;
    }

    j = i;
    while(stash[j])
        j++;
    new_stash = (char *)malloc(sizeof(char) * (j - i + 1));
    if(!new_stash)
        return NULL;
    j = 0;
    while(stash[i])
        new_stash[j++] = stash[i++];
    new_stash[j] = '\0';
    free(stash);
    return new_stash;
}
