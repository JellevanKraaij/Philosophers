#include "philo.h"
#include "mutex.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

int	philo_sim_stop(t_philo *philo_data)
{
	int	_sim_running;

	if (mutex_lock(&philo_data->shared->sim_mutex) < 0)
		return (-1);
	_sim_running = philo_data->shared->sim_running;
	if (mutex_unlock(&philo_data->shared->sim_mutex) < 0)
		return (-1);
	return (!_sim_running);
}

int	philo_print(t_philo *philo_data, char *str, int check_stop)
{
	unsigned int	time;
	int				sim_stop;
	int				ret;

	ret = 0;
	if (ft_gettime_ms(&time))
		return (-1);
	time -= philo_data->shared->sim_start_time;
	if (mutex_lock(&philo_data->shared->print_mutex) < 0)
		return (-1);
	sim_stop = 0;
	if (check_stop)
		sim_stop = philo_sim_stop(philo_data);
	if (sim_stop < 0)
		ret = -1;
	if (!sim_stop)
	{
		if (printf("%u %u %s\n", time, philo_data->name, str) < 0)
			ret = -1;
	}
	if (mutex_unlock(&philo_data->shared->print_mutex) < 0)
		return (-1);
	return (ret);
}

void	*philo_set_error(t_philo *philo_data)
{
	mutex_lock(&philo_data->shared->sim_mutex);
	philo_data->shared->sim_running = 0;
	philo_data->shared->sim_exit_code = 1;
	mutex_unlock(&philo_data->shared->sim_mutex);
	return (NULL);
}

/**
 * @brief take fork
 *
 * @param fork
 * @return int 0 in case of SUCCESS, 1 incase of USED fork, -1 if ERROR
 */
int	philo_take_fork(t_fork *fork)
{
	int	have_fork;

	have_fork = 0;
	while (1)
	{
		if (mutex_lock(&fork->mutex) < 0)
			return (-1);
		if (!fork->in_hand)
		{
			fork->in_hand = 1;
			have_fork = 1;
		}
		if (mutex_unlock(&fork->mutex) < 0)
			return (-1);
		if (have_fork)
			break ;
		usleep(PHILO_YIELD_US);
	}
	return (0);
}

/**
 * @brief drop fork
 *
 * @param fork
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_drop_fork(t_fork *fork)
{
	if (mutex_lock(&fork->mutex) < 0)
		return (-1);
	fork->in_hand = 0;
	if (mutex_unlock(&fork->mutex) < 0)
		return (-1);
	return (0);
}

int	philo_update_eat_stats(t_philo *philo_data)
{
	unsigned int	time;

	if (ft_gettime_ms(&time) < 0)
		return (-1);
	if (mutex_lock(&philo_data->mutex) < 0)
		return (-1);
	philo_data->times_eaten++;
	philo_data->last_eaten = time - philo_data->shared->sim_start_time;
	if (mutex_unlock(&philo_data->mutex))
		return (-1);
	return (0);
}
