/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:13 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 05:20:38 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_init(t_dongle *d, int id, int capacity, t_sched sched,
		long start)
{
	d->heap = malloc(sizeof(t_waiter *) * capacity);
	if (!d->heap)
		return (0);
	d->id = id;
	d->owner = -1;
	d->available_at = start;
	d->stopped = 0;
	d->size = 0;
	d->capacity = capacity;
	d->sched = sched;
	if (pthread_mutex_init(&d->lock, NULL) != 0)
		return (free(d->heap), 0);
	if (sem_init(&d->wake_sem, 0, 0) != 0)
	{
		pthread_mutex_destroy(&d->lock);
		free(d->heap);
		return (0);
	}
	return (1);
}

void	dongle_destroy(t_dongle *d)
{
	pthread_mutex_destroy(&d->lock);
	sem_destroy(&d->wake_sem);
	free(d->heap);
	d->heap = NULL;
}

void	dongle_release(t_sim *sim, t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->owner = -1;
	d->available_at = now_ms() + sim->cooldown;
	pthread_mutex_unlock(&d->lock);
	sem_post(&d->wake_sem);
}

void	stop_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		sim->dongles[i].stopped = 1;
		pthread_mutex_unlock(&sim->dongles[i].lock);
		sem_post(&sim->dongles[i].wake_sem);
		i++;
	}
}

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

int	dongle_acquire(t_sim *sim, t_dongle *d, t_waiter *w)
{
	struct timespec	ts;
	int				ret;

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
		pthread_mutex_unlock(&d->lock);
		ms_to_timespec(now_ms() + 1, &ts);
		ret = sem_timedwait(&d->wake_sem, &ts);
		pthread_mutex_lock(&d->lock);
		if (ret != 0)
			continue ;
	}
	if (w->in_heap)
		heap_remove(d, w);
	pthread_mutex_unlock(&d->lock);
	return (-1);
}
