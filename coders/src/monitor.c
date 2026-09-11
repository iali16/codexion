/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:21 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 03:07:43 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_burned_out(t_sim *sim, t_coder *c)
{
	long	now;

	now = now_ms();
	if (c->state == STATE_WAITING)
		return (0);
	if (now - c->last_compile_start >= sim->burnout)
		return (1);
	return (0);
}

static int	find_burned_coder(t_sim *sim)
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

static int	all_done(t_sim *sim)
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
static void	end_simulation(t_sim *sim)
{
	sim_set_stop(sim);
	stop_dongles(sim);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		burned;
	int		num;

	sim = (t_sim *)arg;
	while (sim_is_running(sim))
	{
		pthread_mutex_lock(&sim->lock);
		burned = find_burned_coder(sim);
		if (burned != -1)
		{
			num = sim->coders[burned].num;
			sim->burned_coder = num;
			pthread_mutex_unlock(&sim->lock);
			log_event(sim, num, "burned out");
			end_simulation(sim);
			return (NULL);
		}
		if (all_done(sim))
		{
			pthread_mutex_unlock(&sim->lock);
			end_simulation(sim);
			return (NULL);
		}
		pthread_mutex_unlock(&sim->lock);
		sleep_ms(sim, 1);
	}
	return (NULL);
}

