#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main()
{
    int fd;
    char    *line;

    int i;

    fd = open("big_line.txt", O_CREAT | O_WRONLY | O_TRUNC, 0777);
    i = 0;
    while(i < 20000)
    {
        write(fd, "A", 1);
        i++;
    }
    write(fd, "\n", 1);
    close(fd);

    printf("Devasa satır okunuyor....");
    fd = open("big_line.txt", O_RDONLY);
    line = get_next_line(fd);
    if(line)
    {
        printf("Okundu! satirin basarili okunan uzunluğu: %d\n", line_len(line));
        free(line);
    }
    close(fd);
}