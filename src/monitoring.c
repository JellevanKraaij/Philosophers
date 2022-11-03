/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitoring.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:02:13 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:02:13 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "mutex.h"
#include <unistd.h>
#include <stdio.h>

/**
 * @brief set simulation end flag
 *
 * @param params
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	simulation_set_finished(t_params *params)
{
	if (mutex_lock(&params->sim_mutex) < 0)
		return (-1);
	params->sim_running = 0;
	if (mutex_unlock(&params->sim_mutex) < 0)
		return (-1);
	return (0);
}

/**
 * @brief check and handle philosopher dead and check finished meals
 *
 * @param[in] philo	philosopher
 * @param[in] params program parameters
 * @param[out] finished is set to true if philosopher finished its meals
 * @param[out] died is set to true if philospher died
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	check_philo(t_philo *philo, \
	t_params *params, int *finished, int *died)
{
	unsigned int	last_eaten;
	unsigned int	times_eaten;
	unsigned int	time;
	int				res;

	res = 0;
	if (mutex_lock(&philo->mutex) < 0)
		return (-1);
	last_eaten = philo->last_eaten;
	times_eaten = philo->times_eaten;
	if (ft_gettime_ms(&time) < 0)
		res = -1;
	if ((time - params->sim_start_time) - last_eaten > params->time_to_die)
	{
		if (res < 0 || simulation_set_finished(params) < 0)
			res = -1;
		if (res < 0 || philo_print(philo, DIED_STR, 0) < 0)
			res = -1;
		*died = 1;
	}
	if (params->times_must_eat_enabled && times_eaten >= params->times_must_eat)
		*finished = 1;
	if (mutex_unlock(&philo->mutex) < 0)
		return (-1);
	return (res);
}

/**
 * @brief check if a philosophers has died or all philosophers are
 * 		  finished with their meals
 *
 * @param[in] params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	check_all_philos(t_params *params)
{
	unsigned int	i;
	unsigned int	finished_cnt;
	int				finished;
	int				died;

	i = 0;
	finished_cnt = 0;
	while (i < params->number_philos)
	{
		finished = 0;
		died = 0;
		if (check_philo(&params->philo_data[i], params, &finished, &died))
			return (-1);
		if (died)
			return (1);
		finished_cnt += finished == 1;
		i++;
	}
	if (finished_cnt == params->number_philos)
	{
		if (simulation_set_finished(params) < 0)
			return (-1);
		return (1);
	}
	return (0);
}

/**
 * @brief check if theire is an error while running simulation
 *
 * @param[in] params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	check_simulation_error(t_params *params)
{
	int	return_code;

	if (mutex_lock(&params->sim_mutex))
		return (-1);
	return_code = params->sim_exit_error;
	if (mutex_unlock(&params->sim_mutex))
		return (-1);
	return (return_code);
}

/**
 * @brief monitor simulation (deaths & finished meals & errors)
 *
 * @param[in] params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	monitor_simulation(t_params *params)
{
	int				philos_result;
	unsigned int	last_ms;
	unsigned int	current;

	if (TIMEOUT_CHECK && ft_gettime_ms(&last_ms) < 0)
		return (-1);
	while (1)
	{
		philos_result = check_all_philos(params);
		if (philos_result)
			return (philos_result);
		if (check_simulation_error(params))
			return (-1);
		if (TIMEOUT_CHECK && ft_gettime_ms(&current) < 0)
			return (-1);
		if (TIMEOUT_CHECK && current - last_ms > 10)
		{
			printf("philo_death_check timeout (%ums)\n", current - last_ms);
			return (-1);
		}
		if (TIMEOUT_CHECK)
			last_ms = current;
		if (usleep(PHILO_YIELD_US) < 0)
			return (-1);
	}
}
