#include "philo.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int	create_threads(pthread_t *threads, unsigned int count, \
	void *(*start_routine)(void *), t_philo *philo_data)
{
	if (count == 0)
		return (0);
	if (pthread_create(threads, NULL, start_routine, philo_data) < 0)
		return (-1);
	if (create_threads(threads + 1, count - 1, start_routine, \
		philo_data + 1) < 0)
	{
		pthread_detach(*threads);
		return (-1);
	}
	return (0);
}

int	init_threads(pthread_t **threads, unsigned int count, \
	void *(*start_routine)(void *), t_philo *philo_data)
{
	*threads = ft_calloc(count, sizeof(pthread_t));
	if (*threads == NULL)
		return (-1);
	if (create_threads(*threads, count, start_routine, philo_data))
	{
		free(*threads);
		threads = NULL;
		return (-1);
	}
	return (0);
}

static void	init_single_philo(t_philo *philo_data, t_params *params, \
	unsigned int name)
{
	philo_data->name = name;
	philo_data->shared = params;
	philo_data->fork_count = 1;
	philo_data->left_fork = &params->forks[name];
	if (name + 1 < params->number_philos)
		philo_data->right_fork = &params->forks[name + 1];
	else
		philo_data->right_fork = &params->forks[0];
	mutex_init(&philo_data->mutex);
}

int	init_philo_data(t_philo **philo_data, unsigned int count, t_params *params)
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

int	simulation_set_finished(t_params *params)
{
	if (mutex_lock(&params->sim_mutex) < 0)
		return (-1);
	params->sim_running = 0;
	if (mutex_unlock(&params->sim_mutex) < 0)
		return (-1);
	return (0);
}

int	check_philo(t_philo *philo, t_params *params, int *died, int *finished)
{
	unsigned int	last_eaten;
	unsigned int	times_eaten;
	unsigned int	time;

	if (mutex_lock(&philo->mutex) < 0)
		return (-1);
	last_eaten = philo->last_eaten;
	times_eaten = philo->times_eaten;
	if (mutex_unlock(&philo->mutex) < 0)
		return (-1);
	if (ft_gettime_ms(&time) < 0)
		return (-1);
	*died = 0;
	*finished = 0;
	if ((time - params->sim_start_time) - last_eaten > params->time_to_die)
		*died = 1;
	if (params->times_must_eat_enabled && times_eaten >= params->times_must_eat)
		*finished = 1;
	return (0);
}

int	check_all_philos(t_params *params)
{
	unsigned int	i;
	int				died;
	int				finished;
	unsigned int	finished_cnt;

	i = 0;
	finished_cnt = 0;
	while (i < params->number_philos)
	{
		if (check_philo(&params->philo_data[i], params, &died, &finished) < 0)
			return (-1);
		if (died)
		{
			simulation_set_finished(params);
			if (philo_print(&params->philo_data[i], DIED_STR, 0) < 0)
				return (-1);
			return (1);
		}
		if (finished)
			finished_cnt++;
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

int	check_simulation_error(t_params *params)
{
	int	return_code;

	if (mutex_lock(&params->sim_mutex))
		return (-1);
	return_code = params->sim_exit_error;
	if (mutex_unlock(&params->sim_mutex))
		return (-1);
	return (return_code);
}

int	monitor_simulation(t_params *params)
{
	int	philos_result;

	while (1)
	{
		philos_result = check_all_philos(params);
		if (philos_result)
			return (philos_result);
		if (check_simulation_error(params))
			return (-1);
		if (usleep(PHILO_YIELD_US) < 0)
			return (-1);
	}
}

int	end_threads(t_params *params, int gracefull)
{
	unsigned int	i;
	int				exit_code;

	i = 0;
	while (i < params->number_philos)
	{
		if (gracefull)
			exit_code = pthread_join(params->threads[i], NULL);
		else
			exit_code = pthread_detach(params->threads[i]);
		if (exit_code < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	run_simulation(t_params *params)
{
	if (ft_gettime_ms(&params->sim_start_time))
		return (-1);
	if (init_philo_data(&params->philo_data, params->number_philos, params) < 0)
		return (-1);
	params->sim_running = 1;
	if (init_threads(&params->threads, params->number_philos, \
		philo_routine, params->philo_data) < 0)
	{
		params->sim_running = 0;
		end_threads(params, 0);
		return (-1);
	}
	if (monitor_simulation(params) < 0)
	{
		end_threads(params, 0);
		return (-1);
	}
	end_threads(params, 1);
	return (0);
}
