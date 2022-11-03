/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_eat.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:02:21 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:02:21 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>
#include "mutex.h"
#include <stdio.h>
/**
 * @brief check if sim should stop or a timeout has occurred
 *
 * @param philo_data
 * @param start_time start time of takefork function
 * @param timeout timeout when a deathlock has occurred
 * @return int 0 in case of CONTINUE, 1 in case of SIMSTOP,-1 if ERROR
 */
static int	philo_fork_check_timeout(t_philo *philo_data, \
	unsigned int start_time, unsigned int timeout)
{
	unsigned int	current_ms;
	int				sim_stop;

	if (ft_gettime_ms(&current_ms) < 0)
		return (-1);
	if (current_ms - start_time > timeout)
	{
		mutex_lock(&philo_data->shared->sim_mutex);
		if (philo_data->shared->sim_running)
		{
			printf("%u %u deathlock occurred\n", \
			current_ms - philo_data->shared->sim_start_time, philo_data->name);
		}
		sim_stop = !philo_data->shared->sim_running;
		philo_data->shared->sim_running = 0;
		mutex_unlock(&philo_data->shared->sim_mutex);
		if (sim_stop)
			return (1);
		return (-1);
	}
	return (0);
}

/**
 * @brief try to take fork
 *
 * @param fork
 * @return int 0 in case of FORK USED, 1 in case of SUCESS, -1 if ERROR
 */
static int	philo_try_take_fork(t_fork *fork)
{
	int	have_fork;

	have_fork = 0;
	if (mutex_lock(&fork->mutex) < 0)
		return (-1);
	if (!fork->in_hand)
	{
		fork->in_hand = 1;
		have_fork = 1;
	}
	if (mutex_unlock(&fork->mutex) < 0)
		return (-1);
	return (have_fork);
}

/**
 * @brief take fork
 *
 * @param fork
 * @return int 0 in case of SUCCESS, -1 if ERROR 1 incase of SIMSTOP
 */
int	philo_take_fork(t_fork *fork, t_philo *philo_data)
{
	int				have_fork;
	int				ret_val;
	unsigned int	start_ms;

	have_fork = 0;
	if (ft_gettime_ms(&start_ms) < 0)
		return (-1);
	while (1)
	{
		ret_val = philo_sim_stop(philo_data);
		if (ret_val)
			return (ret_val);
		ret_val = philo_try_take_fork(fork);
		if (ret_val < 0)
			return (-1);
		if (ret_val)
			return (0);
		ret_val = philo_fork_check_timeout(philo_data, start_ms, \
			philo_data->shared->time_to_die + 10);
		if (ret_val)
			return (ret_val);
		if (usleep(PHILO_YIELD_US))
			return (-1);
	}
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
