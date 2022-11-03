/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_table.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:21:41 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:21:41 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "mutex.h"
#include <unistd.h>

/**
 * @brief wait until a seat is available and go sit down at the table
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, 1 in case of STOP, -1 if ERROR
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
