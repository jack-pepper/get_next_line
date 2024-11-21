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

// Seems good, but the LAST iteration (when no more bytes to read but still
// text in the stash) does not work yet.

#include "get_next_line.h"
#include <stdio.h> // DEBUG!
#include <fcntl.h>

int main(void)
{
	char	*next_line;
	int	fd;

	fd = open("test.txt", O_RDONLY);
	int	i = 0;
	while (1)
	{
		i++;
		printf("=== ITERATION %d === \n", i);
		next_line = get_next_line(fd);
		if (next_line == NULL)
			break ;
		printf("Final next_line: %s\n", next_line);
		free(next_line);
	}
	return (0);
}

char	*get_next_line(int fd)
{
	char		*next_line;
	char		*buf;
	static char	*stash = NULL;
	int		found_line;
	char		*cursor; // is actually useless. Or could be passed to seize_line to be used there

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	printf("[1] Buf successfully mallocated!\n");
	ft_memset(buf, 0, BUFFER_SIZE + 1);
	next_line = NULL;
	cursor = NULL;
	found_line = 0;
	while (!found_line)
	{
		if (read_and_stash(fd, buf, &stash) == 0)
			return (NULL);
		cursor = ft_strchr(stash, '\n');
	//	printf("cursor: %p\n", cursor);
	//	printf("Before the cursor loop...\n");
		if (cursor != NULL)
		{
	//		printf("We enter the cursor loop...\n");
			next_line = seize_line(next_line, &stash, cursor);
			found_line = 1;
		}
		printf("[6] value of found_line = %d\n", found_line);
	}
	// clean up stash, free memory...//
	printf("[end] Complete! next_line: %s - stash: %s\n", next_line, stash);
	return (next_line);
}

char	*seize_line(char *next_line, char **stash, char *cursor)
{
	int	chunk_len;
//	int	i;
	char	*leftover;

	chunk_len = (cursor - *stash) + 1;
	printf("[5] chunk_len = %d\n", chunk_len);
	next_line = malloc(sizeof(char) * (chunk_len + 1));
	if (!next_line)
		return (NULL);
//	i = 0;
	/*
	while ((i < chunk_len) && ((*stash)[i] != '\0'))
	{
		next_line[i] = (*stash)[i];
		i++;
	}
	next_line[i] = '\0';
	*/
	ft_strlcpy(next_line, *stash, chunk_len + 1);
	//leftover = &(*stash)[i];
	leftover = cursor + 1;
	//ft_memset(*stash, 0, i);
	*stash = leftover;
	printf("[5-2] next_line: %s\n", next_line);
	printf("[5-3] stash: %s\n", *stash);
	return (next_line);
}

int	read_and_stash(int fd, char *buf, char **stash)
{
	size_t	b_read;

	b_read = read(fd, buf, BUFFER_SIZE);
	printf("[2] value of b_read = %ld\n", b_read);
	if (b_read <= 0)
	{
		printf("[2-1] No bytes read!\n");
		return (0);
	}
	buf[b_read] = '\0';
	*stash = stash_manager(stash, buf, b_read);
	if (!*stash)
		return (0);
	return (1);
	// DEBUG
	printf("[after reading and stash] Stash: %s\n", *stash); 
	//printf("Stash[0]: %02x\n", (unsigned char)(*stash)[0]);
	//printf("Stash[1]: %02x\n", (unsigned char)(*stash)[1]);
	//printf("Stash[b_read - 1]: %02x\n", (unsigned char)(*stash)[b_read - 1]);
}

char	*stash_manager(char **stash, char *buf, size_t b_read)
{
	char	*new_stash;
	int	stash_len;

	printf("[4] Entering the function stash_manager: %p\n", *stash);
	if (!*stash)
	{
		*stash = malloc(sizeof(char) * (b_read + 1));
		if (*stash == NULL)
			return (NULL);
		ft_memset(*stash, 0, b_read + 1); // might be useless
		ft_strlcpy(*stash, buf, b_read + 1);
		(*stash)[b_read] = '\0';
		printf("[4-new] Stash successfully mallocated and initialized! : %d\n", (*stash)[3]);
		return (*stash);
	}
	stash_len = ft_strlen(*stash);
	printf("[4-realloc] stash_len: %d", stash_len);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1));
	if (new_stash == NULL)
		return (NULL);
	printf("[4-realloc success] New stash malloc success!");
	ft_memset(new_stash, 0, stash_len + b_read + 1);
	ft_strlcpy(new_stash, *stash, stash_len + 1);
	ft_strlcpy(new_stash + stash_len, buf, b_read + 1);
	new_stash[stash_len + b_read] = '\0';
	*stash = new_stash;
	return (*stash);
}
