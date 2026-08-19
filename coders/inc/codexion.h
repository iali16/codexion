/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: drakotov <drakotov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 00:10:10 by drakotov          #+#    #+#             */
/*   Updated: 2026/08/19 02:11:47 by drakotov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# define _DEFAULT_SOURCE

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <time.h>
# include <unistd.h>

typedef enum e_sched
{
	SCHED_FIFO_CUSTOM,
	SCHED_EDF_CUSTOM
}						t_sched;

typedef enum e_state
{
	STATE_WAITING,
	STATE_COMPILING,
	STATE_DEBUGGING,
	STATE_REFACTORING
}						t_state;

typedef struct s_sim	t_sim;
typedef struct s_dongle	t_dongle;
typedef struct s_coder	t_coder;

typedef struct s_waiter
{
	int					coder_id;
	long				seq;
	long				deadline;
	int					in_heap;
}						t_waiter;

struct					s_dongle
{
	int					id;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	int					owner;
	long				available_at;
	int					stopped;
	t_waiter			**heap;
	int					size;
	int					capacity;
	t_sched				sched;
};

struct					s_coder
{
	int					id;
	int					num;
	pthread_t			thread;
	long				last_compile_start;
	long				compile_count;
	t_state				state;
	t_sim				*sim;
};

struct					s_sim
{
	int					n;
	long				burnout;
	long				compile;
	long				debug;
	long				refactor;
	long				required;
	long				cooldown;
	t_sched				sched;
	long				start;
	int					running;
	int					stop_flag;
	int					burned_coder;
	pthread_mutex_t		lock;
	pthread_mutex_t		log_lock;
	pthread_t			monitor;
	t_dongle			*dongles;
	t_coder				*coders;
	long				seq_counter;
};

/* output.c */
void					put_string(const char *s);
void					put_number(long n);

/* time.c */
long					now_ms(void);
void					ms_to_timespec(long ms, struct timespec *ts);

/* log.c */
void					log_event(t_sim *sim, int coder_num, const char *event);

/* parser.c */
int						parse_args(int argc, char **argv, t_sim *sim);

/* sim_utils.c */
int						sim_is_running(t_sim *sim);
int						sleep_ms(t_sim *sim, long ms);

/* scheduler.c */
/* All heap functions are static - no prototypes needed */

/* dongle.c */
int						dongle_init(t_dongle *d, int id, int capacity,
							t_sched sched, long start);
void					dongle_destroy(t_dongle *d);
void					dongle_release(t_sim *sim, t_dongle *d);
int						dongle_acquire(t_sim *sim, t_dongle *d, t_waiter *w);
void					stop_dongles(t_sim *sim);

/* coder.c */
void					make_waiter(t_sim *sim, t_coder *coder, t_waiter *w);
void					coder_release_two(t_sim *sim, t_coder *coder, int first,
							int second);
int						coder_acquire_two(t_sim *sim, t_coder *coder,
							int *first, int *second);
void					*coder_routine(void *arg);

/* monitor.c */
void					*monitor_routine(void *arg);

/* init.c */
int						init_sim(t_sim *sim);
void					cleanup_sim(t_sim *sim);

#endif
