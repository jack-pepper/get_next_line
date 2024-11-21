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

// l.55: need to do seize_line;

#include "get_next_line.h"
#include <stdio.h> // DEBUG!

int main(void)
{
	char	*next_line;

	while (1)
	{
		next_line = get_next_line(0);
		if (next_line == NULL)
			break ;
		printf("%s\n", next_line);
		free(next_line);
	}
}

char	*get_next_line(int fd)
{
	char		*next_line;
	char		*buf;
	static char	*stash = NULL;
	int		found_line;
	char		*cursor;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	printf("Buf successfully mallocated!\n"); // DEBUG
	ft_memset(buf, 0, BUFFER_SIZE + 1);
	next_line = NULL;
	found_line = 0;
	while (!found_line)
	{
		read_and_stash(fd, buf, &stash);
		cursor = ft_strchr(stash, '\n');
		if (cursor != NULL)
		{
			next_line = seize_line();
			found_line = 1;
		}
		printf("value of found_line = %d\n", found_line);
	}
	return (next_line);
}

void	read_and_stash(int fd, char *buf, char **stash)
{
	size_t	b_read;

	b_read = read(fd, buf, BUFFER_SIZE);
	printf("value of b_read = %ld\n", b_read); // DEBUG
	if (b_read <= 0)
	{
		printf("No bytes read!\n"); // DEBUG
		return ;
	}
	buf[b_read] = '\0';
	*stash = stash_manager(*stash, buf, b_read); // add buf content to stash. // this one can be converted to list later...;
	if (!stash)
		return ;
	// DEBUG
	printf("Stash: %s\n", *stash); // DEBUG
	// END DEBUG
}

char	*stash_manager(char *stash, char *buf, size_t b_read)
{
	char	*new_stash;
	int	stash_len;

	if (!stash) // if no stash: for first use or if it has been free already because not needed anymore
	{
		stash = malloc(sizeof(char) * (b_read + 1));
		if (stash == NULL)
			return (NULL);
		ft_memset(stash, 0, b_read + 1);
		ft_strlcpy(stash, buf, b_read);
		printf("Stash successfully mallocated and initialized!\n"); // DEBUG
		return (stash);
	}
	stash_len = ft_strlen(stash);
	new_stash = malloc(sizeof(char) * (stash_len + b_read + 1));
	if (new_stash == NULL)
		return (NULL);
	ft_memset(new_stash, 0, stash_len + b_read + 1);
	ft_strlcpy(new_stash, stash, stash_len + 1);
	ft_strlcpy(new_stash + stash_len, buf, b_read); // that's all that should happen if the stash was empty
	free(stash);
	return (new_stash);
}
