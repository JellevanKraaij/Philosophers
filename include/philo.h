/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/30 23:41:24 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/03/30 23:41:24 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>

# include "mutex.h"

# define TAKEFORK_STR	"has taken a fork"
# define EATING_STR		"is eating"
# define SLEEPING_STR	"is sleeping"
# define THINKING_STR	"is thinking"
# define DIED_STR		"died"

# define PHILO_YIELD_US	200

# ifndef TIMEOUT_CHECK
#  define TIMEOUT_CHECK 0
# endif

typedef struct s_params	t_params;

typedef struct s_fork
{
	t_mutex	mutex;
	int		in_hand;
}	t_fork;

typedef struct s_philo
{
	unsigned int	name;
	unsigned int	times_eaten;
	unsigned int	last_eaten;
	unsigned int	fork_count;
	t_mutex			mutex;
	t_params		*shared;
	t_fork			*left_fork;
	t_fork			*right_fork;
}	t_philo;

typedef struct s_params
{
	unsigned int	number_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	times_must_eat;
	int				times_must_eat_enabled;

	unsigned int	sim_start_time;

	int				sim_running;
	int				sim_exit_error;
	t_mutex			sim_mutex;

	t_mutex			print_mutex;

	unsigned int	table_count;
	t_mutex			table_mutex;

	t_philo			*philo_data;
	pthread_t		*threads;
	t_fork			*forks;
}	t_params;

int		params_init(t_params *params, int argc, char **argv);
int		parse_params(int argc, char **argv,	t_params *params);
int		params_destroy(t_params *params);

int		start_simulation(t_params *params);
int		monitor_simulation(t_params *params);
int		end_simulation(t_params *params);
int		simulation_set_finished(t_params *params);

void	*philo_routine(void *arg);
typedef int				(*t_stopf)(void *);
int		philo_sim_stop(t_philo *philo_data);
int		philo_print(t_philo *philo_data, char *str, int check_stop);
void	*philo_set_error(t_philo *philo_data);
int		philo_update_eat_stats(t_philo *philo_data);
int		philo_sit_down_at_table(t_philo *philo_data);
int		philo_leave_table(t_philo *philo_data);
int		philo_take_fork(t_fork *fork, t_philo *philo_data);
int		philo_drop_fork(t_fork *fork);
int		print_time_protected(unsigned int time, unsigned int philo_name, \
			char *str, t_mutex *print_mutex);

int		init_threads(pthread_t **threads, unsigned int count, \
			void *(*start_routine)(void *), t_philo *philo_data);
int		end_threads(t_params *params);

int		forks_create(t_fork **forks, unsigned int count);
int		forks_destroy(t_fork *forks, unsigned int count);

int		ft_gettime_ms(unsigned int *millis);
int		ft_sleep_ms(unsigned int ms, int (*stop_sleeping_f)(void *), void *arg);
void	*ft_calloc(size_t count, size_t size);
int		ft_atoui_safe(const char *str, unsigned int *num);

#endif
