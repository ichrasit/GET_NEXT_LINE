#include "get_next_line.h"

static char	*read_file(int fd, char *stash)
{
	char	*buf;
	int		bytes;
	int		len;

	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	bytes = 1;
	while (bytes > 0)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1) // Hata durumunda memory leak önlemi
		{
			free(buf);
			free(stash);
			return (NULL);
		}
		buf[bytes] = '\0';
		
		// yeni okunan küçük buff taramaca
		len = line_len(buf); 
		stash = join_stash(stash, buf);
		
		// eğer yeni okunan parçada newline varsa döngüyü mahvet
		if (len > 0 && buf[len - 1] == '\n')
			break ;
	}
	free(buf);
	return (stash);
}
char    *get_next_line(int fd)
{
    static char *stash;
    char *line;

    if(fd < 0 || BUFFER_SIZE <= 0)
        return NULL;
    stash = read_file(fd, stash);
    if(!stash)
        return NULL;
    line = extract_line(stash);
    stash = clean_stash(stash);
    return line;
}