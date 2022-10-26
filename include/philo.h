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

typedef struct s_params	t_params;

typedef struct s_philo
{
	unsigned int	name;
	t_params		*shared;
}	t_philo;

typedef struct s_fork
{
	t_mutex	mutex;
	int		in_hand;
}	t_fork;

int		forks_create(t_fork **forks, unsigned int count);
void	forks_destroy(t_fork *forks, unsigned int count);

typedef struct s_params
{
	unsigned int	number_philos;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	times_must_eat;

	unsigned int	sim_start_time;

	int				sim_running;
	int				sim_exit_code;
	t_mutex			sim_mutex;

	t_mutex			print_mutex;

	t_philo			*philo_data;
	pthread_t		*threads;
	t_fork			*forks;
}	t_params;

int		parse_params(int argc, char **argv,	t_params *philo);
int		params_init(t_params *params);
int		params_destroy(t_params *params);

int		run_simulation(t_params *params);
void	*philo_routine(void *arg);

int		print_time_protected(unsigned int time, unsigned int philo_name, \
			char *str, t_mutex *print_mutex);
int		gettime_ms(unsigned int *millis);
void	*ft_calloc(size_t count, size_t size);
int		ft_atoui_safe(const char *str, unsigned int *num);

#endif
