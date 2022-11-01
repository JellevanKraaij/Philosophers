#include "philo.h"
#include <unistd.h>
#include "mutex.h"

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

/**
 * @brief update philosopher meal simulation information
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
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

/**
 * @brief wait until a seat is available and go sit down at the table
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_sit_down_at_table(t_philo *philo_data)
{
	int	sim_stop;

	while (1)
	{
		if (mutex_lock(&philo_data->shared->table_mutex) < 0)
			return (-1);
		if (philo_data->shared->table_count < \
			philo_data->shared->number_philos - 1)
		{
			philo_data->shared->table_count++;
			if (mutex_unlock(&philo_data->shared->table_mutex) < 0)
				return (-1);
			break ;
		}
		if (mutex_unlock(&philo_data->shared->table_mutex) < 0)
			return (-1);
		if (usleep(PHILO_YIELD_US) < 0)
			return (-1);
		sim_stop = philo_sim_stop(philo_data);
		if (sim_stop < 0)
			return (-1);
		if (sim_stop)
			return (1);
	}
	return (0);
}

/**
 * @brief leave the table
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_leave_table(t_philo *philo_data)
{
	if (mutex_lock(&philo_data->shared->table_mutex) < 0)
		return (-1);
	philo_data->shared->table_count--;
	if (mutex_unlock(&philo_data->shared->table_mutex) < 0)
		return (-1);
	return (0);
}
