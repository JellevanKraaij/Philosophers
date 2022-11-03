/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   params.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:02:17 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:02:17 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include "mutex.h"
#include <stdlib.h>

/**
 * @brief free philosophers data
 *
 * @param params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	destroy_philos(t_params *params)
{
	unsigned int	i;
	int				ret;

	ret = 0;
	if (params->philo_data == NULL)
		return (0);
	i = 0;
	while (i < params->number_philos)
	{
		if (mutex_destroy(&params->philo_data[i].mutex) < 0)
			ret = -1;
		i++;
	}
	free(params->philo_data);
	return (ret);
}

/**
 * @brief free program parameters
 *
 * @param[in] params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	params_destroy(t_params *params)
{
	int	ret;

	ret = 0;
	if (forks_destroy(params->forks, params->number_philos) < 0)
		ret = -1;
	free(params->forks);
	if (mutex_destroy(&params->sim_mutex) < 0)
		ret = -1;
	if (mutex_destroy(&params->table_mutex) < 0)
		ret = -1;
	if (destroy_philos(params) < 0)
		ret = -1;
	free(params->threads);
	return (ret);
}

/**
 * @brief init single philosopher data
 *
 * @param philo_data philosopher that will be initialized
 * @param[in] params program parameters
 * @param[in] name philosopher name (index)
 */
static void	init_single_philo(t_philo *philo_data, t_params *params, \
	unsigned int idx)
{
	philo_data->name = idx + 1;
	philo_data->shared = params;
	philo_data->fork_count = 1;
	philo_data->left_fork = &params->forks[idx];
	if (idx + 1 < params->number_philos)
		philo_data->right_fork = &params->forks[idx + 1];
	else
		philo_data->right_fork = &params->forks[0];
	mutex_init(&philo_data->mutex);
}

/**
 * @brief allocate philosophers data
 *
 * @param[out] philo_data array with philosophers data
 * @param[in] count	amount of philosophers that should be allocated
 * @param[in] params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	init_philos_data(t_philo **philo_data, \
	unsigned int count, t_params *params)
{
	unsigned int	i;

	*philo_data = ft_calloc(count, sizeof(t_philo));
	if (*philo_data == NULL)
		return (-1);
	i = 0;
	while (i < count)
	{
		init_single_philo(&(*philo_data)[i], params, i);
		i++;
	}
	i = 0;
	while (i < count)
	{
		if (mutex_create(&(*philo_data)[i].mutex) < 0)
			return (-1);
		i++;
	}
	return (0);
}

/**
 * @brief initializes program parameters
 *
 * @param params program parameters
 * @param argc program argument count
 * @param argv program arguments
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	params_init(t_params *params, int argc, char **argv)
{
	int	parse_result;

	parse_result = parse_params(argc, argv, params);
	if (parse_result)
		return (parse_result);
	mutex_init(&params->sim_mutex);
	mutex_init(&params->table_mutex);
	mutex_init(&params->print_mutex);
	if (mutex_create(&params->sim_mutex) < 0)
		return (-1);
	if (mutex_create(&params->table_mutex) < 0)
		return (-1);
	if (mutex_create(&params->print_mutex) < 0)
		return (-1);
	if (forks_create(&params->forks, params->number_philos) < 0)
		return (-1);
	if (init_philos_data(&params->philo_data, \
		params->number_philos, params) < 0)
		return (-1);
	return (0);
}
