/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 01:47:29 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 02:20:02 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static int	parse_nonnegative(const char *s, long *out)
{
	long	val;
	int		i;
	int		digit;

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
		if (val > (LONG_MAX - digit) / 10)
			return (0);
		val = val * 10 + digit;
		i++;
	}
	*out = val;
	return (1);
}

static int	parse_scheduler(const char *s, t_sched *sched)
{
	if (strcmp(s, "fifo") == 0)
	{
		*sched = SCHED_FIFO_CUSTOM;
		return (1);
	}
	if (strcmp(s, "edf") == 0)
	{
		*sched = SCHED_EDF_CUSTOM;
		return (1);
	}
	return (0);
}

static int	parse_all_numbers(char **argv, t_args *args)
{
	if (!parse_nonnegative(argv[1], &args->n) || args->n <= 0
		|| args->n > INT_MAX)
		return (0);
	if (!parse_nonnegative(argv[2], &args->burnout))
		return (0);
	if (!parse_nonnegative(argv[3], &args->compile))
		return (0);
	if (!parse_nonnegative(argv[4], &args->debug))
		return (0);
	if (!parse_nonnegative(argv[5], &args->refactor))
		return (0);
	if (!parse_nonnegative(argv[6], &args->required))
		return (0);
	if (!parse_nonnegative(argv[7], &args->cooldown))
		return (0);
	return (1);
}

int	parse_args(int argc, char **argv, t_sim *sim)
{
	t_args	args;

	if (argc != 9)
		return (0);
	if (!parse_all_numbers(argv, &args))
		return (0);
	if (!parse_scheduler(argv[8], &sim->sched))
		return (0);
	sim->n = (int)args.n;
	sim->burnout = args.burnout;
	sim->compile = args.compile;
	sim->debug = args.debug;
	sim->refactor = args.refactor;
	sim->required = args.required;
	sim->cooldown = args.cooldown;
	return (1);
}
