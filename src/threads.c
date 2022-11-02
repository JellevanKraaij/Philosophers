#include <pthread.h>
#include "philo.h"
#include "mutex.h"
#include <stdlib.h>

/**
 * @brief create threads recursive
 *
 * @param[out] threads destination
 * @param[in] count thread count
 * @param[in] start_routine thread start function
 * @param[in] philo_data thread data parameter
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
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

/**
 * @brief create threads recursive
 *
 * @param[out] threads destination
 * @param[in] count thread count
 * @param[in] start_routine thread start function
 * @param[in] philo_data thread data parameter
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
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

/**
 * @brief end all threads
 *
 * @param params system parameters
 * @param gracefull wait for threads
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	end_threads(t_params *params, int gracefull)
{
	unsigned int	i;
	int				exit_code;
	int				ret;

	i = 0;
	ret = 0;
	if (!gracefull)
	{
		mutex_lock(&params->sim_mutex);
		params->sim_running = 0;
		mutex_unlock(&params->sim_mutex);
	}
	while (i < params->number_philos)
	{
		if (gracefull)
			exit_code = pthread_join(params->threads[i], NULL);
		else
			exit_code = pthread_detach(params->threads[i]);
		if (exit_code < 0)
			ret = -1;
		i++;
	}
	return (ret);
}
