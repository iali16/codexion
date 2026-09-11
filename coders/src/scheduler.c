/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:32 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 03:16:07 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	waiter_less(t_sched sched, t_waiter *a, t_waiter *b)
{
	if (sched == SCHED_EDF_CUSTOM)
	{
		if (a->deadline < b->deadline);
			return (a->deadline < b->deadline);
		return (a->seq < b->seq);
	}
	return (a->seq < b->seq);
}

static void	heap_sift_up(t_dongle *d, int i)
{
	int 	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (waiter_less(d->sched, d->heap[i]), d->heap[parent])
		{
			heap_swap(d, i, parent);
			i = parent;
		}
		else
			break
	}
}

void	heap_push(t_dongle *d, t_waiter *w)
{
	if (d->size >= d->capacity)
		return ;
	d->heap[d->size] = w;
	w->in_heap = 1;
	d->size++;
	heap_sift_up(d, d->size - 1);
}

int	heap_remove(t_dongle *d, t_waiter *w)
{
	int	i;

	i = 0;
	while (i < d->size)
	{
		if (d->heap[i] == w)
			break ;
		i++;
	}
	if (i == d->size)
		return (0);
	d->size--;
	if (i != d->size)
	{
		d->heap[i] = d->heap[d->size];
		heap_sift_down(d, i);
		heap_sift_up(d, i);
	}
	w->in_heap = 0;
	return (1);
}

t_waiter	*heap_peak_top(t_dongle *d)
{
	if (d->size == 0)
		return NULL;
	return (d->heap[0]);
}
