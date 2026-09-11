/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2026/08/19 02:27:18 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 12:00:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	sim_get_stop_flag(t_sim *sim)
{
	int	flag;

	pthread_mutex_lock(&sim->lock);
	flag = sim->stop_flag;
	pthread_mutex_unlock(&sim->lock);
	return (flag);
}

void	sim_set_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->lock);
	sim->running = 0;
	sim->stop_flag = 1;
	pthread_mutex_unlock(&sim->lock);
}

int	sim_is_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->lock);
	running = sim->running;
	pthread_mutex_unlock(&sim->lock);
	return (running);
}

int	sleep_ms(t_sim *sim, long ms)
{
	struct timespec	ts;

	ms_to_timespec(ms, &ts);
	while (nanosleep(&ts, &ts) == -1 && errno == EINTR)
	{
		if (!sim_is_running(sim))
			return (-1);
	}
	return (0);
}
