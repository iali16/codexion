/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:13 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:46:33 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	dongle_init(t_dongle *d, t_sim *sim, int id)
{
	d->heap = malloc(sizeof(t_waiter *) * sim->n);
	if (!d->heap)
		return (0);
	d->id = id;
	d->owner = -1;
	d->available_at = sim->start;
	d->stopped = 0;
	d->size = 0;
	d->capacity = sim->n;
	d->sched = sim->sched;
	if (pthread_mutex_init(&d->lock, NULL) != 0)
		return (free(d->heap), 0);
	if (pthread_cond_init(&d->cond, NULL) != 0)
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
	pthread_cond_destroy(&d->cond);
	free(d->heap);
	d->heap = NULL;
}

void	dongle_release(t_sim *sim, t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->owner = -1;
	d->available_at = now_ms() + sim->cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->lock);
}

void	stop_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n)
	{
		pthread_mutex_lock(&sim->dongles[i].lock);
		sim->dongles[i].stopped = 1;
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].lock);
		i++;
	}
}
