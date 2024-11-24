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

// I will try to improve the logic in seize_line.
// Maybe I should free stash there if everything has been copied
// and there is nothing left

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
		else
		{
			printf("Final next_line: *%s*\n", next_line);
			free(next_line);
		}
	}
	return (0);
}
*/
char	*get_next_line(int fd)
{
	char		*buf;
	static char	*stash = NULL;
	int			b_read;
	char		*cursor;

	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		if (stash)
		{
			free(stash);
			stash = NULL;
		}	
		return (NULL);
	}
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	cursor = NULL;
	if (stash != NULL)
		cursor = ft_strchr(stash, '\n');
	while (cursor == NULL)
	{
		b_read = read(fd, buf, BUFFER_SIZE);
		if (b_read == -1)
		{
			free(buf);
			if (stash)
			{
				free(stash);
				stash = NULL;
			}
			return (NULL);
		}
		if (b_read <= 0)
			break ;
		buf[b_read] = '\0';
		stash = stash_manager(stash, buf, b_read);
		if (!stash)
			return (NULL);
		cursor = ft_strchr(stash, '\n');
	}
	free(buf);
	return (seize_line(&stash, cursor));
}

char	*stash_manager(char *stash, char *buf, size_t b_read)
{
	char	*new_stash;
	size_t		stash_len;

	stash_len = 0;
	if (stash)
		stash_len = ft_strlen(stash);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1)); // This is not freed
	if (!new_stash)
		return (NULL);
	if (stash)
	{
		ft_strlcpy(new_stash, stash, stash_len + 1);
		free(stash);
	}
	if (buf)
		ft_strlcpy(new_stash + stash_len, buf, b_read + 1);
	return (new_stash);
}

char	*seize_line(char **stash, char *cursor)
{
	int		chunk_len;
	char	*next_line;
	int	leftover;
	
	next_line = NULL;
	if (*stash && **stash)
	{
		if (cursor != NULL)
		{
			chunk_len = (cursor - *stash + 1);
			next_line = malloc(sizeof(char) * (chunk_len + 1));
			if (!next_line)
				return (NULL);
			ft_strlcpy(next_line, *stash, chunk_len + 1);
			leftover = ft_strlen(*stash) - chunk_len;
			if (leftover > 0)
				ft_strlcpy(*stash, *stash + chunk_len, leftover + 1);
			else
			{
				free(*stash);
				*stash = NULL;
			}
		}
                else  // No newline found, copy everything in stash as the next line: seize_eof
                {
                        chunk_len = ft_strlen(*stash);
                        next_line = malloc(sizeof(char) * (chunk_len + 1));
                        if (!next_line)
                                return (NULL);
                        ft_strlcpy(next_line, *stash, chunk_len + 1);
                        free(*stash);
                        *stash = NULL;
                }
        }
	return (next_line);
}
