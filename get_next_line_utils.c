/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: muhaoz <muhaoz@student.42kocaeli.com.tr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 06:45:57 by muhaoz            #+#    #+#             */
/*   Updated: 2026/08/25 06:50:39 by muhaoz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	line_len(char *str)
{
	int	i;
	int	is_nl;

	i = 0;
	is_nl = 0;
	if (!str)
		return (0);
	while (str[i] && !is_nl)
	{
		is_nl = !(str[i] ^ 10);
		i++;
	}
	return (i);
}

char	*extract_line(char *stash)
{
	char	*line;
	int		len;
	int		i;

	if (!stash || !stash[0])
		return (NULL);
	len = line_len(stash);
	line = (char *)malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	i = 0;
	while (i < len)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*clean_stash(char *stash)
{
	int		i;
	int		j;
	char	*new_stash;

	i = line_len(stash);
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	j = i;
	while (stash[j])
		j++;
	new_stash = (char *)malloc(sizeof(char) * (j - i + 1));
	if (!new_stash)
		return (NULL);
	j = 0;
	while (stash[i])
		new_stash[j++] = stash[i++];
	new_stash[j] = '\0';
	free(stash);
	return (new_stash);
}

char	*join_stash(char *stash, char *buf)
{
	char	*new_stash;
	int		i;
	int		j;

	if (!stash)
	{
		stash = (char *)malloc(1 * sizeof(char));
		stash[0] = '\0';
	}
	if (!stash || !buf)
		return (NULL);
	new_stash = (char *)malloc(sizeof(char) * (line_len(stash) + line_len(buf)
				+ 1));
	if (!new_stash)
		return (NULL);
	i = -1;
	while (stash[++i])
		new_stash[i] = stash[i];
	j = 0;
	while (buf[j])
		new_stash[i++] = buf[j++];
	new_stash[i] = '\0';
	return (free(stash), new_stash);
}
