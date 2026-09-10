/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:06 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 05:22:40 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	make_waiter(t_sim *sim, t_coder *coder, t_waiter *w)
{
	pthread_mutex_lock(&sim->lock);
	sim->seq_counter++;
	w->seq = sim->seq_counter;
	w->deadline = coder->last_compile_start + sim->burnout;
	pthread_mutex_unlock(&sim->lock);
	w->coder_id = coder->id;
	w->in_heap = 0;
}

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

static void	get_dongle_order(t_sim *sim, t_coder *coder, int *first, int *second)
{
	int	left;
	int	right;

	if (sim->n == 1)
	{
		left = 0;
		right = 0;
	}
	else
	{
		if (coder->id == 0)
			left = sim->n - 1;
		else
			left = coder->id - 1;
		right = coder->id;
	}
	if (left <= right)
	{
		*first = left;
		*second = right;
	}
	else
	{
		*first = right;
		*second = left;
	}
}

static int	acquire_first_dongle(t_sim *sim, t_coder *coder, int first)
{
	t_waiter	w1;

	make_waiter(sim, coder, &w1);
	if (dongle_acquire(sim, &sim->dongles[first], &w1) != 0)
		return (-1);
	if (sim_get_stop_flag(sim))
	{
		dongle_release(sim, &sim->dongles[first]);
		return (-1);
	}
	log_event(sim, coder->num, "has taken a dongle");
	return (0);
}

static int	acquire_second_dongle(t_sim *sim, t_coder *coder, int first, int second)
{
	t_waiter	w2;

	if (sim->n == 1)
	{
		log_event(sim, coder->num, "has taken a dongle");
		return (0);
	}
	make_waiter(sim, coder, &w2);
	if (dongle_acquire(sim, &sim->dongles[second], &w2) != 0)
	{
		dongle_release(sim, &sim->dongles[first]);
		return (-1);
	}
	if (sim_get_stop_flag(sim))
	{
		coder_release_two(sim, coder, first, second);
		return (-1);
	}
	log_event(sim, coder->num, "has taken a dongle");
	return (0);
}

int	coder_acquire_two(t_sim *sim, t_coder *coder, int *first, int *second)
{
	get_dongle_order(sim, coder, first, second);
	if (acquire_first_dongle(sim, coder, *first) != 0)
		return (-1);
	if (acquire_second_dongle(sim, coder, *first, *second) != 0)
		return (-1);
	return (0);
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

static void	coder_compile(t_sim *sim, t_coder *coder, int first, int second)
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

static int	coder_after_compile(t_sim *sim, t_coder *coder)
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

static int	coder_debug(t_sim *sim, t_coder *coder)
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

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;
	int		first;
	int		second;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (sim_is_running(sim))
	{
		if (coder_acquire_two(sim, coder, &first, &second) != 0)
			break ;
		if (!sim_is_running(sim))
		{
			coder_release_two(sim, coder, first, second);
			break ;
		}
		coder_compile(sim, coder, first, second);
		if (!sim_is_running(sim))
			break ;
		if (!coder_after_compile(sim, coder))
			break ;
		if (!coder_debug(sim, coder))
			break ;
		if (!coder_refactor(sim, coder))
			break ;
	}
	return (NULL);
}
