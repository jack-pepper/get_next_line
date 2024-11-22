/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:33:12 by mmalie            #+#    #+#             */
/*   Updated: 2024/11/19 09:34:54 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h> // DEBUG
#include <fcntl.h>

/*
int	main(void)
{
	char	*next_line;
	int		fd;
	int		i;

	fd = open("test.txt", O_RDONLY);
	i = 0;
	while (1)
	{
		i++;
		printf("=== ITERATION %d === \n", i);
		next_line = get_next_line(fd);
		if (next_line == NULL)
		{
			printf("End of file!\n");
			break ;
		}
		printf("Final next_line: %s\n", next_line);
		free(next_line);
	}
	return (0);
}
*/

char	*get_next_line(int fd)
{
	char		*next_line;
	char		*buf;
	static char	*stash = NULL;
	int			exit;
	char		*cursor;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	ft_memset(buf, 0, BUFFER_SIZE + 1);
	next_line = NULL;
	cursor = NULL;
	exit = 0;
	while (exit == 0)
	{
		exit = read_and_stash(fd, buf, &stash);
		if (exit == -1)
			return (NULL);
		else if (exit == 0)
			cursor = ft_strchr(stash, '\n');
		else if (exit == 1)
			cursor = NULL;
		if (cursor != NULL || exit == 1)
		{
			next_line = seize_line(next_line, &stash, cursor);
			exit = 1;
		}
	}
	// clean up stash, free memory...//
	return (next_line);
}

int	read_and_stash(int fd, char *buf, char **stash)
{
	ssize_t	b_read;

	b_read = read(fd, buf, BUFFER_SIZE);
	if (b_read < 0)
		return (-1);
	if (b_read == 0)
	{
		if (*stash[0] != '\0')
		{
			return (1);
		}
		return (-1);
	}
	buf[b_read] = '\0';
	*stash = stash_manager(stash, buf, b_read);
	if (!*stash)
		return (-1);
	return (0);
}

char	*stash_manager(char **stash, char *buf, size_t b_read)
{
	char	*new_stash;
	int		stash_len;

	if (!*stash)
	{
		*stash = malloc(sizeof(char) * (b_read + 1));
		if (*stash == NULL)
			return (NULL);
		ft_memset(*stash, 0, b_read + 1);
		ft_strlcpy(*stash, buf, b_read + 1);
		(*stash)[b_read] = '\0';
		return (*stash);
	}
	stash_len = ft_strlen(*stash);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1));
	if (new_stash == NULL)
		return (NULL);
	ft_memset(new_stash, 0, stash_len + b_read + 1);
	ft_strlcpy(new_stash, *stash, stash_len + 1);
	ft_strlcpy(new_stash + stash_len, buf, b_read + 1);
	new_stash[stash_len + b_read] = '\0';
	*stash = new_stash;
	return (*stash);
}

char	*seize_line(char *next_line, char **stash, char *cursor)
{
	int		chunk_len;
	char	*leftover;
	int		i;

	i = 0;
	if (cursor == NULL)
	{
		cursor = *stash;
		while ((*stash)[i] != '\0')
		{
			cursor++;
			i++;
		}
	}
	chunk_len = (cursor - *stash + 1);
	next_line = malloc(sizeof(char) * (chunk_len + 1));
	if (!next_line)
		return (NULL);
	ft_strlcpy(next_line, *stash, chunk_len + 1);
	leftover = &(*stash)[i];
	leftover = cursor + 1;
	*stash = leftover;
	return (next_line);
}
