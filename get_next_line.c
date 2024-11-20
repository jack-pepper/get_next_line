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
#include <stdio.h> // DEBUG!

int main(void)
{
	char	*next_line;

	next_line = get_next_line(0);
	printf("%s", next_line);
}

char	*get_next_line(int fd)
{
	char		*next_line;
	static t_list	*stash = NULL;

	if ((fd < 0) || (BUFF_SIZE <= 0))
		return (NULL);
	next_line = NULL;
	read_fd(fd, &stash);
	// 2. extract from stash to line
	seize_line(next_line, stash);
	// 3. clean up stash
	if (next_line[0] == '\0')
	{
		// free stash
		// stash = NULL'
		// free(line);
		// return (NULL);
	}
	return (next_line);
}

void	read_fd(int fd, t_list **stash)
{
	int	b_read;
	char	*buf;
	int	found_line;

	found_line = 0;
	while (found_line == 0)
	{
		parse_for_newline(stash, &found_line);
		buf = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf)
			return ;
		b_read = read(fd, buf, BUFF_SIZE);
		if ((b_read == 0 && *stash == NULL) || (b_read == -1))
		{
			free(buf);
			return ;
		}
		buf[b_read] = '\0';
		add_to_stash(stash, buf, b_read);
		free(buf);
	}
}

void	add_to_stash(t_list **stash, char *buf, int b_read)
{
	t_list	*last;
	t_list	*new_node;
	int	i;

	new_node = malloc(sizeof(t_list));
	if (!new_node)
		return ;
	new_node->content = malloc(sizeof(char) * (b_read + 1));
	if (!new_node->content)
		return ;
	new_node->next = NULL;
	i = 0;
	while ((buf[i] != '\0') && (i < b_read))
	{
		new_node->content[i] = buf[i];
		i++;
	}
	new_node->content[i] = '\0';
	if (*stash == NULL)
	{
		*stash = new_node;
		return ;
	}
	last = ft_lst_getlast(*stash);
	last->next = new_node;
}

void	parse_for_newline(t_list *stash, int *found_line)
{
	int	i;
	t_list	*last_node;

	if (stash == NULL)
		return (NULL);
	last_node = ft_lst_get_last(stash);
	i = 0;
	while (last_node->content[i] != '\0')
	{
		if (last_node->content[i] == '\n')
		{
			*found_line = 1;
			return ;
		}
		i++;
	}
	*found_line = 0;
}

////////
void	seize_line(char *next_line, t_list *stash)
{
	int	i;
	int	j;

	i = 0;
	while (stash != NULL)
	{
		j = 0;
		if (stash->content[j] != '\n')
		{
			next_line[i] = stash->content[j];
			i++;
			j++;
		}
		stash = stash->next;
	}


}

/*
char	*get_next_line(int fd)
{
	char	*next_line;
	static char	*buf = NULL;
	size_t	b_read;
	int	line_seized;
	char	*chunk_end;
	size_t	chunk_size;
	t_list	*head;

	if (fd < 0)
		return (NULL);
	if (!buf)
	{
		buf = malloc(sizeof(char) * (BUFF_SIZE + 1));
		if (!buf)
			return (NULL);
	}
	chunk_end = NULL;
	head = NULL;
	chunk_size = 0;
	line_seized = 0;
	while (!line_seized)
	{
		if (buf[0] == '\0')
		{
			ft_cut_and_paste(head, chunk_end, chunk_end - buf);
		}
		b_read = read(fd, buf, BUFF_SIZE);
		if ((ssize_t)b_read == -1)
			return (NULL);
		chunk_end = ft_strchr(buf, '\n');
		if ((!chunk_end) && (b_read == BUFF_SIZE))
			ft_cut_and_paste(head, buf, chunk_size);
		else
		{
			if (b_read == 0)
				return (NULL);
			else if (chunk_end)
				chunk_size = chunk_end - buf;
			else if ((!chunk_end) && (b_read < BUFF_SIZE))
				chunk_size = (ft_strchr(buf, '\0')) - buf;
			ft_cut_and_paste(head, buf, chunk_size);
			line_seized = 1;
		}
	}
	next_line = fetch_line(next_line, head);
	return (next_line);
}

char	*fetch_line(char *dest, t_list *lst)
{
	char	*next_line;
	int	lst_size;
	t_list	*head;

	head = lst;
	while (lst != NULL)
	{
		lst_size++;
		lst = lst->next;
	}
	next_line = (char *)malloc(sizeof(t_list) * (lst_size + 1));
	if (!next_line)
		return (NULL);
	while (head != NULL)
	{
		ft_strlcpy(next_line, head->content, ft_strlen(head->content));
		head = head->next;
	}
	return (next_line);
}

t_list	*ft_cut_and_paste(t_list *head, char *buf, size_t len)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
		return (head);
	new_node->content = (char *)malloc(len + 1);
	if (new_node->content == NULL)
		return (head);
	ft_strlcpy(new_node->content, buf, len);
	((char *)new_node->content)[len] = '\0';
	head->next = new_node;
	new_node->next = NULL;
	ft_memset(buf, 0, len);
	return (new_node);
}

*/

/*
char	*get_next_line(int fd)
{
	char	*newline;
	int	line_seized;
	char	*buf;
	char	*buf_end;
	t_list	*head;

	buf = (char *)malloc(sizeof(char) * BUFF_SIZE);
	if (buf == NULL)
		return (NULL);
	ft_memset(buf, 0, BUFF_SIZE);
	buf_end = &buf[BUFF_SIZE];
	head = NULL;
	newline = NULL;
	line_seized = 0;
	while (!line_seized)
	{
		head = parse_fd(buf, buf_end, head, &line_seized);
	}
	ft_lstcpy(newline, head);
	return (newline);
}

char	*parse_fd(char *buf, char *buf_end, t_list *head, int *line_seized)
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
			*line_seized = 1;
			chunk_len = buf[BUFF_SIZE - 1] - chunk_end;
		}
		// calloc for node... then copy the content
		node = ft_lstnew(chunk_len); // copy all if b_read == BUFF_SIZE, else copy up to \n
		if (head != NULL)
			ft_lstadd_back(head, node);
		else
		{
			head = node;
			ft_memset(buf, 0, chunk_len);
			if (b_read < BUFF_SIZE)
				*line_seized = 1;
		}
	}
	return (head);
}

char	*ft_lstcpy(char *dest, t_list *lst)
{
	while (*lst != NULL)
	{
		ft_strlcpy(dest, lst->content, BUFF_SIZE);
		lst = lst->next;
	}
	return (dest);
}
*/
