#include "philo.h"
#include "mutex.h"
#include <stdlib.h>

void	destroy_philo_mutex(t_params *params)
{
	unsigned int	i;

	if (params->philo_data)
		return ;
	i = 0;
	while (i < params->number_philos)
	{
		mutex_destroy(&params->philo_data[i].mutex);
		i++;
	}
	free(params->philo_data);
}

int	params_destroy(t_params *params)
{
	forks_destroy(params->forks, params->number_philos);
	free(params->forks);
	mutex_destroy(&params->sim_mutex);
	mutex_destroy(&params->table_mutex);
	destroy_philo_mutex(params);
	free(params->philo_data);
	free(params->threads);
	return (-1);
}

int	params_init(t_params *params)
{
	mutex_init(&params->sim_mutex);
	mutex_init(&params->table_mutex);
	if (mutex_create(&params->sim_mutex) < 0)
		return (-1);
	if (mutex_create(&params->table_mutex) < 0)
		return (-1);
	if (forks_create(&params->forks, params->number_philos) < 0)
		return (-1);
	return (0);
}
