/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 01:47:29 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 02:00:26 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/codexion.h"

static int	parse_nonnegative(const char *s, long long *out)
{
	long long	val;
	int			i;
	int			digit;

	val = 0;
	i = 0;
	if (!s || !s[i])
		return (0);
	if (s[i] == '+')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		digit = s[i] - '0';
		if (val > (LLONG_MAX - digit) / 10)
			return (0);
		val = val * 10 + digit;
		i++;
	}
	*out = val;
	return (1);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	long long	n;
	long long	burnout;
	long long	compile;
	long long	debug;
	long long	refactor;
	long long	required;
	long long	cooldown;

	if (argc != 9)
		return (0);
	if (!parse_nonnegative(argv[1], &n) || n <= 0 || n > INT_MAX)
		return (0);
	if (!parse_nonnegative(argv[2], &burnout))
		return (0);
	if (!parse_nonnegative(argv[3], &compile))
		return (0);
	if (!parse_nonnegative(argv[4], &debug))
		return (0);
	if (!parse_nonnegative(argv[5], &refactor))
		return (0);
	if (!parse_nonnegative(argv[6], &required))
		return (0);
	if (!parse_nonnegative(argv[7], &cooldown))
		return (0);
	if (strcmp(argv[8], "fifo") == 0)
		sim->sched = SCHED_FIFO;
	else if (strcmp(argv[8], "edf") == 0)
		sim->sched = SCHED_EDF;
	else
		return (0);
	sim->n = (int)n;
	sim->burnout = burnout;
	sim->compile = compile;
	sim->debug = debug;
	sim->refactor = refactor;
	sim->required = required;
	sim->cooldown = cooldown;
	return (1);
}
