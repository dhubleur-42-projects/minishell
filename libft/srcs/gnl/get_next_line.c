/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 20:09:50 by plouvel           #+#    #+#             */
/*   Updated: 2022/02/11 18:28:09 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
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
		free(gnl->start_buffer_addr);
		gnl->flags &= ~(INIT);
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
	static t_gnl	gnl;

	if (!init_gnl(&gnl, fd))
		return (NULL);
	while (read_fd(&gnl) > 0)
	{
		gnl.new_line = ft_strchr(gnl.buffer, '\n');
		fill_line(&gnl);
		if (!gnl.line)
		{
			gnl.readed = 0;
			gnl.flags &= ~(CAN_READ);
			gnl.flags |= MALLOC_EXCEPTION;
		}
	}
	return (quit_gnl(&gnl));
}
