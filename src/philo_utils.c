#include "philo.h"
#include "mutex.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

/**
 * @brief set simulation stop flag
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
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

/**
 * @brief set simulation error flag
 *
 * @param philo_data
 * @return void*
 */
void	*philo_set_error(t_philo *philo_data)
{
	mutex_lock(&philo_data->shared->sim_mutex);
	philo_data->shared->sim_running = 0;
	philo_data->shared->sim_exit_error = 1;
	mutex_unlock(&philo_data->shared->sim_mutex);
	return (NULL);
}

/**
 * @brief print philosopher status (str)
 *
 * @param philo_data
 * @param str status
 * @param check_stop check if simulation is running
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_print(t_philo *philo_data, char *str, int check_stop)
{
	unsigned int	time;
	int				ret;
	int				stop;

	ret = 0;
	if (mutex_lock(&philo_data->shared->print_mutex) < 0)
		return (-1);
	if (ft_gettime_ms(&time) < 0)
		ret = -1;
	time -= philo_data->shared->sim_start_time;
	stop = 0;
	if (check_stop)
		stop = philo_sim_stop(philo_data);
	if (stop < 0)
		ret = -1;
	if (stop == 0 && ret >= 0)
	{
		if (printf("%u %u %s\n", time, philo_data->name, str) < 0)
			ret = -1;
	}
	if (mutex_unlock(&philo_data->shared->print_mutex) < 0 || ret < 0)
		return (-1);
	return (ret);
}
