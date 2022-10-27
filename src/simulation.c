#include "philo.h"
#include <pthread.h>
#include <unistd.h>

int	init_threads(pthread_t **threads, unsigned int count)
{
	*threads = ft_calloc(count, sizeof(pthread_t));
	if (*threads == NULL)
		return (-1);
	return (0);
}

int	create_threads(pthread_t *threads, unsigned int count, \
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

int	init_philo_data(t_philo **philo_data, unsigned int count, t_params *params)
{
	unsigned int	i;

	*philo_data = ft_calloc(count, sizeof(t_philo));
	if (*philo_data == NULL)
		return (-1);
	i = 0;
	while (i < count)
	{
		(*philo_data)[i].name = i;
		(*philo_data)[i].shared = params;
		(*philo_data)[i].fork_count = 1;
		(*philo_data)[i].left_fork = &params->forks[i];
		if (i + 1 < count)
			(*philo_data)[i].right_fork = &params->forks[i + 1];
		else
			(*philo_data)[i].right_fork = &params->forks[0];
		mutex_init(&(*philo_data)[i].mutex);
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
	if (time - last_eaten > params->time_to_die)
		*died = 1;
	if (times_eaten >= params->times_must_eat)
		*finished = 1;
	return (0);
}

int	simulation_set_finished(t_params *params)
{
	if (mutex_lock(&params->sim_mutex) < 0)
		return (-1);
	params->sim_running = 0;
}

int	check_all_philos(t_params *params)
{
	unsigned int	i;
	int				died;
	int				finished;
	int				finished_cnt;

	i = 0;
	finished_cnt = 0;
	while (i < params->number_philos)
	{
		if (check_philo(&params->philo_data[i], params, died, finished) < 0)
			return (-1);
		if (died)
		{
			if (simulation_set_finished(params) < 0 || \
				philo_print(&params->philo_data[i], DIED_STR, 0) < 0)
				return (-1);
			return (1);
		}
		if (finished)
			finished_cnt++;
	}
	if (finished_cnt == params->philo_data->times_eaten)
	{
		if (simulation_set_finished(params) < 0)
			return (-1);
		return (1);
	}
	return (0);
}

void	simulation_monitor_deaths(t_params *params)
{
	while (1)
	{

	}
}

int	run_simulation(t_params *params)
{
	unsigned int	i;

	if (ft_gettime_ms(&params->sim_start_time))
		return (-1);
	if (init_philo_data(&params->philo_data, params->number_philos, params) < 0)
		return (-1);
	if (init_threads(&params->threads, params->number_philos) < 0)
		return (-1);
	params->sim_running = 1;
	if (create_threads(params->threads, params->number_philos, \
		philo_routine, params->philo_data) < 0)
	{
		params->sim_running = 0;
		return (-1);
	}
	simulation_monitor_deaths(params);
	while (i < params->number_philos)
	{
		pthread_join(params->threads[i], NULL);
		i++;
	}
	return (0);
}
