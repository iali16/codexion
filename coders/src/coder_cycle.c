/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_cycle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2026/08/19 01:47:06 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 12:00:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coder_release_two(t_sim *sim, t_coder *coder, int first, int second)
{
	(void)coder;
	if (sim->n == 1)
	{
		dongle_release(sim, &sim->dongles[first]);
		return ;
	}
	dongle_release(sim, &sim->dongles[second]);
	dongle_release(sim, &sim->dongles[first]);
}

static int	check_all_done(t_sim *sim)
{
	int	i;
	int	done;

	done = 1;
	i = 0;
	pthread_mutex_lock(&sim->lock);
	while (i < sim->n)
	{
		if (sim->coders[i].compile_count < sim->required)
		{
			done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->lock);
	return (done);
}

void	coder_compile(t_sim *sim, t_coder *coder, int first, int second)
{
	long	start_time;

	start_time = now_ms();
	pthread_mutex_lock(&sim->lock);
	coder->last_compile_start = start_time;
	coder->state = STATE_COMPILING;
	pthread_mutex_unlock(&sim->lock);
	log_event(sim, coder->num, "is compiling");
	if (sleep_ms(sim, sim->compile) != 0)
	{
		coder_release_two(sim, coder, first, second);
		return ;
	}
	coder_release_two(sim, coder, first, second);
}

int	coder_after_compile(t_sim *sim, t_coder *coder)
{
	int	done;

	pthread_mutex_lock(&sim->lock);
	if (!sim->running)
	{
		pthread_mutex_unlock(&sim->lock);
		return (0);
	}
	coder->compile_count++;
	pthread_mutex_unlock(&sim->lock);
	done = check_all_done(sim);
	if (done)
	{
		sim_set_stop(sim);
		stop_dongles(sim);
		return (0);
	}
	pthread_mutex_lock(&sim->lock);
	coder->state = STATE_DEBUGGING;
	pthread_mutex_unlock(&sim->lock);
	return (1);
}

int	coder_debug(t_sim *sim, t_coder *coder)
{
	log_event(sim, coder->num, "is debugging");
	if (sleep_ms(sim, sim->debug) != 0)
		return (0);
	pthread_mutex_lock(&sim->lock);
	if (!sim->running)
	{
		pthread_mutex_unlock(&sim->lock);
		return (0);
	}
	coder->state = STATE_REFACTORING;
	pthread_mutex_unlock(&sim->lock);
	return (1);
}
