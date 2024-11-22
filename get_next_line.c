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

int	main(void)
{
	char	*next_line;
	int		fd;
	int		i;

	fd = open("test1char.txt", O_RDONLY);
	i = 0;
	while (1)
	{
		i++;
		printf("=== ITERATION %d === \n", i);
		next_line = get_next_line(fd);
		if (next_line == NULL)
		{
			//printf("End of file!\n");
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
		{
			free(buf);
			//free(next_line);
			return (NULL);
		}
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
	free(buf);
	return (next_line);
}

int	read_and_stash(int fd, char *buf, char **stash)
{
	ssize_t	b_read;
	char	*new_stash;

	b_read = read(fd, buf, BUFFER_SIZE);
	printf("b_read: %ld\n", b_read);
	if (b_read < 0)
	{
		return (-1);
	}
	if (b_read == 0)
	{
		if (*stash == NULL)
		{
			return (-1);
		}
		if (*stash != NULL)
		{
			//printf("Stash: *%s* - %p\n", *stash, stash);
			if (ft_strcmp(*stash, "") == 0)
				return (-1);
		}
			return (1);
		/*
		printf("Result ft_strcmp: %d\n", ft_strcmp(*stash, ""));
		if ((*stash != NULL) && (ft_strcmp(*stash, "") == 0))
		{
			printf("empty string\n");	
			return (-1);
		}
		else if ((*stash != NULL) && (ft_strcmp(*stash, "") != 0))
		{
			printf("stash has data: %s\n", *stash);
		}
		return (1);
		*/
	}
	buf[b_read] = '\0';
	new_stash = stash_manager(stash, buf, b_read); // changed from b_read + 1
	if (!new_stash)
	{
		return (-1);
	}
	*stash = new_stash;
	return (0);
}

char	*stash_manager(char **stash, char *buf, size_t b_read)
{
	char	*new_stash;
	size_t		stash_len;

	if (*stash == NULL)
	{
		*stash = malloc(sizeof(char) * (b_read + 1));
		if (*stash == NULL)
			return (NULL);
		//printf("[1] stash len: %ld - content: *%s*\n", ft_strlen(*stash), *stash);
		ft_memset(*stash, 0, b_read + 1);
		ft_strlcpy(*stash, buf, b_read + 1);
		(*stash)[b_read] = '\0';
		printf("[2] stash len: %ld - content: *%s*\n", ft_strlen(*stash), *stash);
		return (*stash);
	}
	stash_len = ft_strlen(*stash);
	//printf("[3] stash len: %ld - content: *%s*\n", ft_strlen(*stash), *stash);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1)); // modified from +1
	if (new_stash == NULL)
	{
		printf("Malloc failed\n");
		return (NULL);
	}
	//printf("Malloc success!\n");
	ft_memset(new_stash, 0, stash_len + b_read + 1);
	ft_strlcpy(new_stash, *stash, stash_len + 1);
	ft_strlcpy(new_stash + stash_len, buf, b_read + 1);
	new_stash[stash_len + b_read] = '\0';
	//free(*stash);
	*stash = new_stash;
	//printf("[4] stash len: %ld - content: *%s*\n", ft_strlen(*stash), *stash);
	//printf("[5] new_stash len: %ld - content: *%s*\n", ft_strlen(new_stash), *stash);
	//*stash = new_stash;
	return (new_stash);
}

char	*seize_line(char *next_line, char **stash, char *cursor)
{
	int		chunk_len;
	char	*leftover;
	int		i;

	i = 0;
	if (cursor == NULL) // = if b_read = 0
	{
		cursor = *stash;
		if ((*stash != NULL && (*stash)[0] != '\0'))
		{
			while ((*stash)[i] != '\0')
			{
				cursor++;
				i++;
			}
			i--;
		}
	}
	chunk_len = (cursor - *stash + 1);
	next_line = malloc(sizeof(char) * (chunk_len + 1));
	if (!next_line)
		return (NULL);
	ft_strlcpy(next_line, *stash, chunk_len + 1);
	//leftover = &(*stash)[i];
	leftover = cursor + 1;
	*stash = leftover;
	return (next_line);
}
