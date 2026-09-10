/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:18 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 05:19:18 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_threads(t_sim *sim, int *created)
{
	*created = 0;
	while (*created < sim->n)
	{
		if (pthread_create(&sim->coders[*created].thread, NULL, coder_routine,
				&sim->coders[*created]) != 0)
			return (0);
		(*created)++;
	}
	return (1);
}

static void	stop_on_error(t_sim *sim)
{
	sim_set_stop(sim);
	stop_dongles(sim);
}

static void	wait_threads(t_sim *sim, int created)
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

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->log_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->lock);
		return (0);
	}
	return (1);
}

static void	handle_init_error(t_sim *sim)
{
	pthread_mutex_destroy(&sim->lock);
	pthread_mutex_destroy(&sim->log_lock);
	write(2, "Allocation error\n", 17);
}

int	main(int argc, char **argv)
{
	t_sim	sim;
	int		created;

	memset(&sim, 0, sizeof(sim));
	if (!parse_args(argc, argv, &sim))
		return (write(2, "Invalid arguments\n", 18), 1);
	if (sim.required == 0)
		return (0);
	sim.start = now_ms();
	sim.running = 1;
	sim.stop_flag = 0;
	if (!init_mutexes(&sim))
		return (1);
	if (!init_sim(&sim))
		return (handle_init_error(&sim), 1);
	if (pthread_create(&sim.monitor, NULL, monitor_routine, &sim) != 0)
		return (cleanup_sim(&sim), 1);
	if (!create_threads(&sim, &created))
		stop_on_error(&sim);
	wait_threads(&sim, created);
	cleanup_sim(&sim);
	return (0);
}
