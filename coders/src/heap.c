/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:32 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:46:26 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	heap_swap(t_dongle *d, int a, int b)
{
	t_waiter	*tmp;

	tmp = d->heap[a];
	d->heap[a] = d->heap[b];
	d->heap[b] = tmp;
}

static void	heap_sift_down(t_dongle *d, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < d->size
			&& waiter_less(d->sched, d->heap[left], d->heap[smallest]))
			smallest = left;
		if (right < d->size
			&& waiter_less(d->sched, d->heap[right], d->heap[smallest]))
			smallest = right;
		if (smallest == i)
			break ;
		heap_swap(d, i, smallest);
		i = smallest;
	}
}

static void	heap_sift_up(t_dongle *d, int i)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (waiter_less(d->sched, d->heap[i], d->heap[parent]))
		{
			heap_swap(d, i, parent);
			i = parent;
		}
		else
			break ;
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
