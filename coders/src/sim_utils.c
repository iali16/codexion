/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 02:27:18 by drakotov          #+#    #+#             */
/*   Updated: 2026/09/11 22:45:32 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_dongles(t_sim *sim, int count)
{
	while (count > 0)
	{
		count--;
		dongle_destroy(&sim->dongles[count]);
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->n);
	if (!sim->dongles)
		return (0);
	memset(sim->dongles, 0, sizeof(t_dongle) * sim->n);
	i = 0;
	while (i < sim->n)
	{
		if (!dongle_init(&sim->dongles[i], sim, i))
		{
			destroy_dongles(sim, i);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n)
	{
		sim->coders[i].id = i;
		sim->coders[i].num = i + 1;
		sim->coders[i].last_compile_start = sim->start;
		sim->coders[i].compile_count = 0;
		sim->coders[i].state = STATE_WAITING;
		sim->coders[i].sim = sim;
		i++;
	}
}

int	init_sim(t_sim *sim)
{
	sim->coders = malloc(sizeof(t_coder) * sim->n);
	if (!sim->coders)
		return (0);
	memset(sim->coders, 0, sizeof(t_coder) * sim->n);
	if (!init_dongles(sim))
	{
		free(sim->coders);
		sim->coders = NULL;
		return (0);
	}
	init_coders(sim);
	return (1);
}

void	cleanup_sim(t_sim *sim)
{
	int	i;

	if (sim->dongles)
	{
		i = 0;
		while (i < sim->n)
		{
			dongle_destroy(&sim->dongles[i]);
			i++;
		}
		free(sim->dongles);
		sim->dongles = NULL;
	}
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	pthread_mutex_destroy(&sim->lock);
	pthread_mutex_destroy(&sim->log_lock);
}
