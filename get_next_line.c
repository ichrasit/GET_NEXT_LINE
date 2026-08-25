#include "get_next_line.h"

static int	flush_chunk(char **stash, char *buf, int *t, int b_size)
{
	int	n_len;

	n_len = line_len(buf);
	if (*t + BUFFER_SIZE > b_size || (n_len > 0 && buf[n_len - 1] == '\n'))
	{
		*stash = join_stash(*stash, buf);
		*t = 0;
		if (n_len > 0 && buf[n_len - 1] == '\n')
			return (0);
	}
	return (1);
}

static char	*read_file(int fd, char *stash, char *buf, int b_size)
{
	int	b;
	int	t;

	t = 0;
	while (1)
	{
		b = read(fd, buf + t, BUFFER_SIZE);
		if (b <= 0)
			break ;
		buf[t + b] = '\0';
		t += b;
		if (!flush_chunk(&stash, buf, &t, b_size))
			break ;
	}
	if (b == -1)
	{
		free(stash);
		return (NULL);
	}
	if (t > 0)
		stash = join_stash(stash, buf);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;
	char		*buf;
	int			b_size;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	b_size = BUFFER_SIZE > 4096 ? BUFFER_SIZE : 4096;
	buf = malloc(sizeof(char) * (b_size + 1));
	if (!buf)
		return (NULL);
	stash = read_file(fd, stash, buf, b_size);
	free(buf);
	if (!stash)
		return (NULL);
	line = extract_line(stash);
	stash = clean_stash(stash);
	return (line);
}