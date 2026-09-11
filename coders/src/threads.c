/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:18 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:45:23 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	create_threads(t_sim *sim, int *created)
{
	*created = 0;
	while (*created < sim->n)
	{
		if (pthread_create(&sim->coders[*created].thread, NULL,
				coder_routine, &sim->coders[*created]) != 0)
			return (0);
		(*created)++;
	}
	return (1);
}

void	stop_on_error(t_sim *sim)
{
	sim_set_stop(sim);
	stop_dongles(sim);
}

void	wait_threads(t_sim *sim, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}
