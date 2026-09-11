/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+ +#+           */
/*   Created: 2026/08/19 01:47:13 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 12:00:00 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	try_acquire(t_sim *sim, t_dongle *d, t_waiter *w)
{
	long	current;

	(void)sim;
	current = now_ms();
	if (d->size > 0 && d->heap[0] == w && d->owner == -1
		&& current >= d->available_at)
	{
		heap_remove(d, w);
		d->owner = w->coder_id;
		return (1);
	}
	return (0);
}

static void	wait_for_turn(t_dongle *d)
{
	struct timespec	ts;

	pthread_mutex_unlock(&d->lock);
	ms_to_timespec(now_ms() + 1, &ts);
	(void)sem_timedwait(&d->wake_sem, &ts);
	pthread_mutex_lock(&d->lock);
}

int	dongle_acquire(t_sim *sim, t_dongle *d, t_waiter *w)
{
	pthread_mutex_lock(&d->lock);
	if (d->stopped || sim_get_stop_flag(sim) || d->size >= d->capacity)
	{
		pthread_mutex_unlock(&d->lock);
		return (-1);
	}
	heap_push(d, w);
	while (!d->stopped && !sim_get_stop_flag(sim))
	{
		if (try_acquire(sim, d, w))
		{
			pthread_mutex_unlock(&d->lock);
			return (0);
		}
		wait_for_turn(d);
	}
	if (w->in_heap)
		heap_remove(d, w);
	pthread_mutex_unlock(&d->lock);
	return (-1);
}
