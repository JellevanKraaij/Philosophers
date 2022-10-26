#include "philo.h"
#include <unistd.h>
#include "mutex.h"
#include <pthread.h>
#include <stdio.h>

int	philo_check_sim_stop(t_philo *philo_data)
{
	int	_sim_running;

	if (mutex_lock(&philo_data->shared->sim_mutex) < 0)
		return (-1);
	_sim_running = philo_data->shared->sim_running;
	if (mutex_unlock(&philo_data->shared->sim_mutex) < 0)
		return (-1);
	if (!_sim_running)
		pthread_exit(NULL);
	return (0);
}

int	*philo_error_exit(t_philo *philo_data)
{
	mutex_lock(&philo_data->shared->sim_mutex);
	philo_data->shared->sim_running = 0;
	philo_data->shared->sim_exit_code = 1;
	mutex_unlock(&philo_data->shared->sim_mutex);
	pthread_exit(NULL);
}

void	*philo_routine(void *arg)
{
	t_philo		*philo_data;

	philo_data = arg;
	while (1)
	{
		unsigned int	time;
		usleep(1000 * 1000);
		gettime_ms(&time);
		print_time_protected(time - philo_data->shared->sim_start_time, philo_data->name, "test", &philo_data->shared->print_mutex);
		philo_check_sim_stop(philo_data);
	}
}
