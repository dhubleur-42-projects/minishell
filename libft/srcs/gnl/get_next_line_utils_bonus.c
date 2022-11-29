/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/30 23:00:47 by plouvel           #+#    #+#             */
/*   Updated: 2022/02/13 13:16:53 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"
#include <stdlib.h>

static inline size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*s++ != '\0')
		len++;
	return (len);
}

static char	*ft_strjoin(char *s1, char *s2)
{
	char	*str;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = (char *) malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1[j] != '\0')
		str[i++] = s1[j++];
	j = 0;
	while (s2[j] != '\0')
		str[i++] = s2[j++];
	str[i] = '\0';
	free(s1);
	return (str);
}

char	*flush_gnl(t_gnl *gnl)
{
	free(gnl->start_buffer_addr);
	gnl->start_buffer_addr = NULL;
	gnl->buffer = NULL;
	gnl->flags &= ~(INIT);
	return (gnl->buffer);
}

void	fill_line(t_gnl *gnl)
{
	if (gnl->new_line)
	{
		gnl->tmp_char = gnl->new_line[1];
		gnl->new_line[1] = '\0';
		gnl->line = ft_strjoin(gnl->line, gnl->buffer);
		gnl->new_line[1] = gnl->tmp_char;
		if (gnl->new_line[1] == '\0')
		{
			gnl->buffer = gnl->start_buffer_addr;
			gnl->flags |= CAN_READ;
		}
		else
		{
			gnl->buffer = &gnl->new_line[1];
			gnl->flags &= ~(CAN_READ);
		}
		gnl->flags |= LINE_DONE;
	}
	else
	{
		gnl->line = ft_strjoin(gnl->line, gnl->buffer);
		gnl->buffer = gnl->start_buffer_addr;
		gnl->flags |= CAN_READ;
	}
}
