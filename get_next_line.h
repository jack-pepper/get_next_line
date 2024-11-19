/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmalie <mmalie@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 09:35:00 by mmalie            #+#    #+#             */
/*   Updated: 2024/11/19 09:36:28 by mmalie           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>

BUFFER_SIZE=42

typedef struct s_list
{
	void			*content;
	struct_s_list	*next;
}				t_list;

char	*get_next_line(int fd);
