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

int main(void)
{
	char	*newline;

	newline = get_next_line(0);
	printf("%s", newline);
}

char	*get_next_line(int fd)
{
	char	*newline;
	int	line_seized;
	char	*buf;
	char	*buf_end;
	t_list	*head;

	buf = (char *)malloc(char * BUFF_SIZE);
	if (buf == NULL)
		return (NULL);
	ft_memset(buf, 0, BUFF_SIZE);
	buf_end = buf[BUFF_SIZE];
	head = NULL;
	newline = NULL;
	found_newline = 0;
	eof = 0;
	while (!line_seized)
	{
		head = parse_fd(buf, buf_end, head, &line_seized);
	}
	ft_lstcopy(newline, head);
	return (newline);
}

char	*parse_fd(char *buf, int *found_newline, int *eof)
{
	size_t	b_read;
	char	*chunk_end;
	size_t	chunk_len;

	b_read = read(fd, buf, BUFF_SIZE);
	if (b_read == 0) // is this part really needed?
		*line_seized = 1;
	else
	{
		chunk_len = BUFF_SIZE - 1;
		chunk_end = ft_strchr(buf, '\n');
		if (chunk_end != NULL)
		{
			*found_newline = 1;
			chunk_len = buf[BUFF_SIZE - 1] - chunk_end;
		}
		node = ft_lstnew(chunk_len); // copy all if b_read == BUFF_SIZE, else copy up to \n
		if (head != NULL)
			ft_lstadd_back(head, node);
		else
			head = node;
			ft_memset(buf, 0, chunk_len);
			if (b_read < BUFF_SIZE)
				*line_seized = 1;
	}
	return (head);
}

char	*ft_lstcopy(char *dest, char *lst)
{
	while (head != NULL)
	{
		ft_strlcpy(newline, head->content, BUFF_SIZE);
		head = head->next;
	}
	return (dest);
}

