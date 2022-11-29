/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp_ignore_case.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouvel <plouvel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 10:58:06 by plouvel           #+#    #+#             */
/*   Updated: 2022/03/30 11:26:19 by plouvel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp_ignore_case(const char *s1, const char *s2)
{
	unsigned int	s1_compnd;
	unsigned int	s2_compnd;

	while (*s1 != '\0')
	{
		s1_compnd = 0;
		s2_compnd = 0;
		if (ft_isupper(*s1))
			s1_compnd = 32;
		if (ft_isupper(*s2))
			s2_compnd = 32;
		if ((*s1) + s1_compnd != (*s2) + s2_compnd)
			break ;
		s1++;
		s2++;
	}
	return (((*(unsigned char *)s1) + s1_compnd)
		- ((*(unsigned char *)s2) + s2_compnd));
}
