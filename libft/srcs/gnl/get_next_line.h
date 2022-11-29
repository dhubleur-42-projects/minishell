/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/11 18:27:47 by plouvel           #+#    #+#             */
/*   Updated: 2022/02/11 18:27:51 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <sys/types.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

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

/* get_next_line_utils.c */

void	fill_line(t_gnl *gnl);

/* get_next_line.c */

char	*get_next_line(int fd);

#endif
