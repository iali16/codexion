/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:13 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:46:43 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	try_acquire(t_dongle *d, t_waiter *w)
{
	long	current;

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

static int	enter_wait(t_sim *sim, t_dongle *d, t_waiter *w)
{
	pthread_mutex_lock(&d->lock);
	if (d->stopped || sim_get_stop_flag(sim) || d->size >= d->capacity)
	{
		pthread_mutex_unlock(&d->lock);
		return (0);
	}
	heap_push(d, w);
	return (1);
}

static void	wait_cooldown(t_sim *sim, t_dongle *d)
{
	long	remaining;

	remaining = d->available_at - now_ms();
	if (remaining <= 0)
		return ;
	pthread_mutex_unlock(&d->lock);
	sleep_ms(sim, remaining);
	pthread_mutex_lock(&d->lock);
}

int	dongle_acquire(t_sim *sim, t_dongle *d, t_waiter *w)
{
	if (!enter_wait(sim, d, w))
		return (-1);
	while (!d->stopped && !sim_get_stop_flag(sim))
	{
		if (try_acquire(d, w))
		{
			pthread_mutex_unlock(&d->lock);
			return (0);
		}
		if (d->owner == -1 && d->heap[0] == w)
		{
			wait_cooldown(sim, d);
			continue ;
		}
		pthread_cond_wait(&d->cond, &d->lock);
	}
	if (w->in_heap)
		heap_remove(d, w);
	pthread_mutex_unlock(&d->lock);
	return (-1);
}
