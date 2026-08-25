#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd;
    char    *line;
    int i;

    fd = open("test.txt", O_RDONLY);
    if(fd == -1)
        return(printf("dosya acilmadi"), 1);
    i = 1;
    while((line = get_next_line(fd)) != NULL)
    {
        printf("satir %d : %s", i, line);
        free(line);
        i++;
    }
    close(fd);
    return 0;
}