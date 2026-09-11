/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_check.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2026/08/19 01:47:21 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 12:00:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burned_out(t_sim *sim, t_coder *c)
{
	long	now;

	now = now_ms();
	if (c->state != STATE_WAITING)
		return (0);
	if (now - c->last_compile_start >= sim->burnout)
		return (1);
	return (0);
}

int	find_burned_coder(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n)
	{
		if (is_burned_out(sim, &sim->coders[i]))
			return (i);
		i++;
	}
	return (-1);
}

int	all_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n)
	{
		if (sim->coders[i].compile_count < sim->required)
			return (0);
		i++;
	}
	return (1);
}
