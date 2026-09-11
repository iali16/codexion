/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:21 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:46:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	end_simulation(t_sim *sim)
{
	sim_set_stop(sim);
	stop_dongles(sim);
}

static int	check_end(t_sim *sim)
{
	int	burned;
	int	num;

	pthread_mutex_lock(&sim->lock);
	burned = find_burned_coder(sim);
	if (burned != -1)
	{
		num = sim->coders[burned].num;
		sim->burned_coder = num;
		pthread_mutex_unlock(&sim->lock);
		log_event(sim, num, "burned out");
		return (1);
	}
	if (all_done(sim))
	{
		pthread_mutex_unlock(&sim->lock);
		return (1);
	}
	pthread_mutex_unlock(&sim->lock);
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (sim_is_running(sim))
	{
		if (check_end(sim))
		{
			end_simulation(sim);
			return (NULL);
		}
		sleep_ms(sim, 1);
	}
	return (NULL);
}
