/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_acquire.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:06 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:47:09 by drakotov         ###   ########.fr       */
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

static void	get_dongle_order(t_coder *coder, int *first, int *second)
{
	int	left;
	int	right;
	int	n;

	n = coder->sim->n;
	if (coder->id == 0)
		left = n - 1;
	else
		left = coder->id - 1;
	right = coder->id;
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

static int	acquire_second_dongle(t_sim *sim, t_coder *coder, int first,
		int second)
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
	get_dongle_order(coder, first, second);
	if (acquire_first_dongle(sim, coder, *first) != 0)
		return (-1);
	if (acquire_second_dongle(sim, coder, *first, *second) != 0)
		return (-1);
	return (0);
}
