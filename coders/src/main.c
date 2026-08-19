/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 01:47:18 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 01:57:41 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_sim	sim;
	int		created;
	int		i;

	memset(&sim, 0, sizeof(sim));
	if (!parse_args(argc, argv, &sim))
	{
		fprintf(stderr, "Invalid arguments\n");
		return (1);
	}
	if (sim.required == 0)
		return (0);
	sim.start = now_ms();
	sim.running = 1;
	sim.stop_flag = 0;
	if (pthread_mutex_init(&sim.lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&sim.log_lock, NULL) != 0)
	{
		pthread_mutex_destroy(&sim.lock);
		return (1);
	}
	if (!init_sim(&sim))
	{
		pthread_mutex_destroy(&sim.lock);
		pthread_mutex_destroy(&sim.log_lock);
		fprintf(stderr, "Allocation error\n");
		return (1);
	}
	if (pthread_create(&sim.monitor, NULL, monitor_routine, &sim) != 0)
	{
		cleanup_sim(&sim);
		return (1);
	}
	created = 0;
	while (created < sim.n)
	{
		if (pthread_create(&sim.coders[created].thread, NULL, coder_routine,
				&sim.coders[created]) != 0)
			break ;
		created++;
	}
	if (created < sim.n)
	{
		pthread_mutex_lock(&sim.lock);
		sim.running = 0;
		sim.stop_flag = 1;
		pthread_mutex_unlock(&sim.lock);
		stop_dongles(&sim);
	}
	i = 0;
	while (i < created)
	{
		pthread_join(sim.coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim.monitor, NULL);
	cleanup_sim(&sim);
	return (0);
}
