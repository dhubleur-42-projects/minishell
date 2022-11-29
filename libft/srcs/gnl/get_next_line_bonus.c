/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 13:20:12 by plouvel           #+#    #+#             */
/*   Updated: 2022/02/13 13:20:14 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>
#include <unistd.h>

static inline ssize_t	read_fd(t_gnl *gnl)
{
	if (gnl->flags & LINE_DONE)
		return (0);
	if (gnl->flags & CAN_READ)
	{
		gnl->readed = read(gnl->fd, gnl->buffer, BUFFER_SIZE);
		if (gnl->readed > 0)
			gnl->buffer[gnl->readed] = '\0';
	}
	return (gnl->readed);
}

static inline char	*ft_strchr(const char *s, int c)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (unsigned char) c)
			return ((char *) &s[i]);
		i++;
	}
	return (NULL);
}

static char	*init_gnl(t_gnl *gnl, int fd)
{
	if (!(gnl->flags & INIT))
	{
		if (fd < 0 || BUFFER_SIZE <= 0)
			return (NULL);
		gnl->buffer = (char *) malloc((BUFFER_SIZE + 1) * sizeof(char));
		if (!gnl->buffer)
			return (NULL);
		gnl->fd = fd;
		gnl->start_buffer_addr = gnl->buffer;
		gnl->flags |= INIT;
		gnl->flags |= CAN_READ;
		gnl->flags &= ~(MALLOC_EXCEPTION);
	}
	gnl->line = (char *) malloc(sizeof(char));
	if (!gnl->line)
	{
		free(gnl->start_buffer_addr);
		return (NULL);
	}
	gnl->line[0] = '\0';
	return (gnl->line);
}

static char	*quit_gnl(t_gnl *gnl)
{
	if (gnl->readed <= 0)
	{
		flush_gnl(gnl);
		if (!(gnl->flags & MALLOC_EXCEPTION))
		{
			if (gnl->line[0] != '\0')
				return (gnl->line);
		}
		free(gnl->line);
		return (NULL);
	}
	gnl->flags &= ~(LINE_DONE);
	return (gnl->line);
}

char	*get_next_line(int fd)
{
	static t_gnl	gnl[OPEN_MAX];

	if (fd == GNL_FLUSH)
		return (flush_gnl(&gnl[gnl->fd]));
	if (!init_gnl(&gnl[fd], fd))
		return (NULL);
	while (read_fd(&gnl[fd]) > 0)
	{
		gnl[fd].new_line = ft_strchr(gnl[fd].buffer, '\n');
		fill_line(&gnl[fd]);
		if (!gnl[fd].line)
		{
			gnl[fd].readed = 0;
			gnl[fd].flags &= ~(CAN_READ);
			gnl[fd].flags |= MALLOC_EXCEPTION;
		}
	}
	return (quit_gnl(&gnl[fd]));
}
