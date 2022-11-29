/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 20:07:41 by plouvel           #+#    #+#             */
/*   Updated: 2022/02/13 13:16:52 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <sys/types.h>

# if defined (__APPLE__)
#  include <limits.h>
# elif __linux__
#  define OPEN_MAX 1024
# endif

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# define GNL_FLUSH -255

typedef enum e_flags
{
	INIT = 1U,
	LINE_DONE = 1U << 1,
	CAN_READ = 1U << 2,
	MALLOC_EXCEPTION = 1U << 3
}				t_flags;

typedef struct s_gnl
{
	char	*buffer;
	char	*start_buffer_addr;
	char	*line;
	char	*new_line;
	char	tmp_char;
	int		fd;
	ssize_t	readed;
	int		flags;
}				t_gnl;

/* get_next_line_utils_bonus.c */

void	fill_line(t_gnl *gnl);
char	*flush_gnl(t_gnl *gnl);

/* get_next_line_bonus.c */

char	*get_next_line(int fd);

#endif
