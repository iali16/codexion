/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:46:56 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 02:14:11 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	put_string(const char *s)
{
	int	len;

	len = 0;
	while (s[len])
		len++;
	(void)write(1, s, len);
}

void	put_number(long n)
{
	char			buf[32];
	unsigned long	mag;
	int				i;
	char			c;

	if (n == 0)
	{
		(void)write(1, "0", 1);
		return ;
	}
	if (n < 0)
	{
		(void)write(1, "-", 1);
		mag = (unsigned long)(-(n + 1)) + 1;
	}
	else
		mag = (unsigned long)n;
	i = 0;
	while (mag > 0 && i < 31)
	{
		buf[i++] = '0' + (mag % 10);
		mag /= 10;
	}
	while (i > 0)
	{
		c = buf[--i];
		(void)write(1, &c, 1);
	}
}
