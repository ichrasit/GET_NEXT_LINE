#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <stdlib.h>
#include <unistd.h>

#ifndef BUFFER_SIZE 
#define BUFFER_SIZE 42
#endif

char    *get_next_line(int fd);

int line_len(char *str);
char    *extract_line(char *stash);
char    *clean_stash(char *stash);
char    *join_stash(char *stash, char *buf);
#endif