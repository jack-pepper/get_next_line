/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:36:54 by mmalie            #+#    #+#             */
/*   Updated: 2024/11/19 09:37:21 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	*ft_strchr(const char *s, int c)
{
	if (c < 0 || c > 255)
		c = c % 256;
	while (*s)
	{
		if (*s == (char)c)
		{
			return ((char *)s);
		}
		s++;
	}
	if (c == '\0')
	{
		return ((char *)s);
	}
	return (NULL);
}

size_t  ft_strlcpy(char *dest, const char *src, size_t size)
{	
	size_t	src_len;
        size_t	i;

	src_len = ft_strlen(src);
	if (size == 0)
		return (src_len);
	if (size == 1)
	{
		dest[0] = '\0';
		return (src_len);
	}
	i = 0;
	while ((i < size - 1) && (src[i] != '\0'))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (src_len);
}

t_list	*ft_lstnew(void *content)
{
	t_list  *new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (new_node == NULL)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list  *last_node;
	
	if (lst == NULL || new == NULL)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last_node = ft_lstlast(*lst);
		last_node->next = new;
	}
}

