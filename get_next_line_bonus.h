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

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <stddef.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

/* get_next_line.c */
char	*get_next_line(int fd);
char	*read_and_stash(int fd, char **buf, char **stash, char **cursor);
char	*stash_manager(char *stash, char *buf, size_t b_read);
char	*seize_line(char **stash, char *cursor);
char	*seize_eof(char **next_line, char **stash);

/* get_next_line_utils.c */
void	*ft_memset(void *s, int c, size_t n);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char *s);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
void	clean_stash(char **stash);

#endif
