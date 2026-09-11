/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2026/08/19 01:47:06 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 12:00:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_refactor(t_sim *sim, t_coder *coder)
{
	log_event(sim, coder->num, "is refactoring");
	if (sleep_ms(sim, sim->refactor) != 0)
		return (0);
	pthread_mutex_lock(&sim->lock);
	if (!sim->running)
	{
		pthread_mutex_unlock(&sim->lock);
		return (0);
	}
	coder->state = STATE_WAITING;
	pthread_mutex_unlock(&sim->lock);
	return (1);
}

static int	coder_step(t_sim *sim, t_coder *coder)
{
	if (!coder_after_compile(sim, coder))
		return (0);
	if (!coder_debug(sim, coder))
		return (0);
	if (!coder_refactor(sim, coder))
		return (0);
	return (1);
}

static int	coder_cycle(t_sim *sim, t_coder *coder)
{
	int	first;
	int	second;

	if (coder_acquire_two(sim, coder, &first, &second) != 0)
		return (0);
	if (!sim_is_running(sim))
	{
		coder_release_two(sim, coder, first, second);
		return (0);
	}
	coder_compile(sim, coder, first, second);
	if (!sim_is_running(sim))
		return (0);
	return (coder_step(sim, coder));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (sim_is_running(sim))
	{
		if (!coder_cycle(sim, coder))
			break ;
	}
	return (NULL);
}
