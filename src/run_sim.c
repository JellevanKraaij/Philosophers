#include "philo.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief run simulation
 *
 * @param params program parameters
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	start_simulation(t_params *params)
{
	if (ft_gettime_ms(&params->sim_start_time))
		return (-1);
	params->sim_running = 1;
	if (init_threads(&params->threads, params->number_philos, \
		philo_routine, params->philo_data) < 0)
	{
		params->sim_running = 0;
		end_threads(params, 0);
		return (-1);
	}
	return (0);
}

/**
 * @brief stop simulation
 *
 * @param params program parameters
 * @param gracefull wait for threads
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	end_simulation(t_params *params, int gracefull)
{
	return (end_threads(params, gracefull));
}
