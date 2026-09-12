/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:32 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/12 02:15:40 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	waiter_less(t_sched sched, t_waiter *a, t_waiter *b)
{
	if (sched == SCHED_EDF_CUSTOM)
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
		return (a->coder_id < b->coder_id);
	}
	return (a->seq < b->seq);
}

t_waiter	*heap_peek_top(t_dongle *d)
{
	if (d->size == 0)
		return (NULL);
	return (d->heap[0]);
}
