#include "philo.h"
#include "mutex.h"
#include <stdlib.h>

int	params_destroy(t_params *params)
{
	free(params->philo_data);
	forks_destroy(params->forks, params->number_philos);
	free(params->forks);
	mutex_destroy(&params->sim_mutex);
	mutex_destroy(&params->print_mutex);
	free(params->threads);
	return (-1);
}

int	params_init(t_params *params)
{
	mutex_init(&params->sim_mutex);
	mutex_init(&params->print_mutex);
	if (forks_create(&params->forks, params->number_philos) < 0)
		return (-1);
	if (mutex_create(&params->print_mutex) < 0)
		return (-1);
	if (mutex_create(&params->sim_mutex) < 0)
		return (-1);
	return (0);
}
