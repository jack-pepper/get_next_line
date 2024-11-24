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
	int			exit;
	char		*cursor;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	ft_memset(buf, 0, BUFFER_SIZE + 1);
	// If there is a stash already we look for a \n
	
	cursor = NULL;
	if (stash != NULL)
		cursor = ft_strchr(stash, '\n');
	// As long as no \n found
	while (cursor == NULL)
	{
		// We 
		exit = read_and_stash(fd, buf, &stash);
		// If b_read <= 0: exit the loop and seize the next_line
		if ((exit == -1) || (exit == 0 && stash == NULL))
		{
			break ;
		}
		// If b_read > 0: we look for \n. We continue the logic until it's find
		else if (exit == 0)
			cursor = ft_strchr(stash, '\n');
	}
	free(buf);
	return (seize_line(&stash, cursor));
}

int	read_and_stash(int fd, char *buf, char **stash)
{
	ssize_t	b_read;
	char	*new_stash;

	// We read the fd
	b_read = read(fd, buf, BUFFER_SIZE);
//	printf("b_read: %ld\n", b_read);
	// If error or no bytes read, we return -1 to exit the loop 
	// and returns a null line
	if (b_read <= 0)
		return (-1);
	buf[b_read] = '\0';
	if (*stash)
		new_stash = stash_manager(stash, buf, b_read); // changed from b_rea
	else
	{
		new_stash = malloc(sizeof(char) * (b_read + 1));
		if (!new_stash)
			return (-1);
		ft_strlcpy(new_stash, buf, b_read + 1);
	}
	*stash = new_stash;
	return (0);
}

char	*stash_manager(char **stash, char *buf, size_t b_read)
{
	char	*new_stash;
	size_t		stash_len;

	// We malloc a new stash of buf size
	stash_len = 0;
	printf("1-stash: *%s* - buf: %s\n", *stash, buf);
	if (*stash)
		stash_len = ft_strlen(*stash);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1));
	if (new_stash == NULL)
		return (NULL);
	if (*stash)
	{
		ft_strlcpy(new_stash, *stash, stash_len + 1);
		//free(*stash);
		printf("2-stash: *%s* 2-new_stash: %s\n", *stash, new_stash);
	}
	if (buf)
	{
		ft_strlcpy(new_stash + stash_len, buf, b_read + 1);
		printf("3-stash: *%s* 3-new_stash: %s\n", *stash, new_stash);
	}
	printf("4-stash: *%s* 4-new_stash: %s\n", *stash, new_stash);
	return (new_stash);
}

char	*seize_line(char **stash, char *cursor)
{
	int		chunk_len;
	char	*next_line;

	next_line = NULL;	
	// If the stash is not NULL
	if (*stash && **stash)
	{
		printf("*stash && **stash found\n");
		// If a \n has been found we calculate the len of the chunk
		// malloc to a next_line and copy the chunk up to the \n
		if (cursor != NULL)
		{	
			printf("cursor is not null\n");
			chunk_len = (cursor - *stash + 1);
			printf("chunk_len: %d\n", chunk_len);
			next_line = malloc(sizeof(char) * (chunk_len + 1));
			if (!next_line)
				return (NULL);
			ft_strlcpy(next_line, *stash, chunk_len + 1);
			next_line[chunk_len] = '\0';
			// Here we set the new stash to the leftover
			*stash = cursor + 1;
			printf("*stash: *%s* - next_line: *%s*\n", *stash, next_line);
		}
		// In case of exit 0, if b_read <= 0, we copy what is left in stash
		else
		{
			printf("cursor is null\n");
			chunk_len = ft_strlen(*stash);
			printf("chunk_len: %d\n", chunk_len);
			next_line = malloc(sizeof(char) * (chunk_len + 1));
			if (!next_line)
				return (NULL);
			ft_strlcpy(next_line, *stash, chunk_len + 1);
			next_line[chunk_len] = '\0';
			printf("*stash: *%s* - next_line: *%s*\n", *stash, next_line);
			free(*stash);
			*stash = NULL;
		}
	}
	// If the stash is NULL: we free it and returns a null next_line
	else
	{
		printf("stash is null\n");
		next_line = NULL;
	}
	return (next_line);
	

}
