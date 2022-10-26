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
	if (create_threads(threads + 1, count - 1, start_routine, philo_data + 1) < 0)
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
		i++;
	}
	return (0);
}

int	run_simulation(t_params *params)
{
	unsigned int	i;

	if (gettime_ms(&params->sim_start_time))
		return (-1);
	if (init_philo_data(&params->philo_data, params->number_philos, params) < 0)
		return (-1);
	if (init_threads(&params->threads, params->number_philos) < 0)
		return (-1);
	params->sim_running = 1;
	if (create_threads(params->threads, params->number_philos, \
		philo_routine, params->philo_data) < 0)
		return (-1);
	i = 0;
	while (i < 15)
	{
		usleep(1000 * 1000);
		i++;
	}
	mutex_lock(&params->sim_mutex);
	params->sim_running = 0;
	mutex_unlock(&params->sim_mutex);
	i = 0;
	while (i < params->number_philos)
	{
		pthread_join(params->threads[i], NULL);
		i++;
	}
	return (0);
}
