/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprintf_puts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 11:26:52 by plouvel           #+#    #+#             */
/*   Updated: 2022/03/30 11:26:55 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"
#include <stdlib.h>
#include <unistd.h>

void	dprintf_putchar(char c, t_printf_info *info)
{
	ft_putchar_fd(c, info->fd);
	info->wlen++;
}

void	dprintf_putstr(const char *str, t_printf_info *info)
{
	size_t	len;

	len = ft_strlen(str);
	write(info->fd, str, len);
	info->wlen += len;
}

void	dprintf_put_buffers(t_printf_info *info)
{
	if (info->bufs.left)
		dprintf_putstr(info->bufs.left, info);
	if (info->bufs.main)
	{
		if (info->conv == 'c' && info->bufs.main[0] == '\0')
			dprintf_putchar('\0', info);
		else
			dprintf_putstr(info->bufs.main, info);
	}
	if (info->bufs.right)
		dprintf_putstr(info->bufs.right, info);
	free(info->bufs.left);
	free(info->bufs.main);
	free(info->bufs.right);
}
