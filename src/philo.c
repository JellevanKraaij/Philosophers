/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:21:46 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:21:46 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include "mutex.h"
#include <pthread.h>
#include <stdio.h>

/**
 * @brief philosopher is thinking
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_thinking(t_philo *philo_data)
{
	if (philo_print(philo_data, THINKING_STR, 1) < 0)
		return (-1);
	return (0);
}

/**
 * @brief philospher is sleeping
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_sleep(t_philo *philo_data)
{
	if (philo_print(philo_data, SLEEPING_STR, 1) < 0)
		return (-1);
	if (ft_sleep_ms(philo_data->shared->time_to_sleep, \
		(t_stopf)philo_sim_stop, philo_data) < 0)
		return (-1);
	return (0);
}

/**
 * @brief philosopher is eating
 *
 * @param philo_data
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	philo_eat(t_philo *philo_data)
{
	if (philo_sit_down_at_table(philo_data) < 0)
		return (-1);
	if (philo_take_fork(philo_data->left_fork, philo_data) < 0)
		return (-1);
	if (philo_print(philo_data, TAKEFORK_STR, 1))
		return (-1);
	if (philo_take_fork(philo_data->right_fork, philo_data) < 0)
		return (-1);
	if (philo_print(philo_data, TAKEFORK_STR, 1))
		return (-1);
	if (philo_update_eat_stats(philo_data))
		return (-1);
	if (philo_print(philo_data, EATING_STR, 1) < 0)
		return (-1);
	if (ft_sleep_ms(philo_data->shared->time_to_eat, \
		(t_stopf)philo_sim_stop, philo_data))
		return (-1);
	if (philo_drop_fork(philo_data->left_fork) < 0 || \
		philo_drop_fork(philo_data->right_fork) < 0)
		return (-1);
	if (philo_leave_table(philo_data) < 0)
		return (-1);
	return (0);
}

/**
 * @brief execute philosopher routines
 *
 * @param arg
 * @return void*
 */
void	*philo_routine(void *arg)
{
	t_philo			*philo_data;

	philo_data = arg;
	while (1)
	{
		if (philo_thinking(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_eat(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_sleep(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_sim_stop(philo_data))
			return (NULL);
	}
}
