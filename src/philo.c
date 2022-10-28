#include "philo.h"
#include <unistd.h>
#include "mutex.h"
#include <pthread.h>
#include <stdio.h>

int	philo_thinking(t_philo *philo_data)
{
	if (philo_print(philo_data, THINKING_STR, 1) < 0)
		return (-1);
	return (0);
}

int	philo_sleep(t_philo *philo_data)
{
	if (philo_print(philo_data, "is sleeping", 1) < 0)
		return (-1);
	if (ft_sleep_ms(philo_data->shared->time_to_sleep, \
		(t_stopf)philo_sim_stop, philo_data) < 0)
		return (-1);
	return (0);
}

int	philo_eat(t_philo *philo_data)
{
	// if (philo_wait_for_seat(philo_data))
	// 	return (-1);
	t_fork	*f1;
	t_fork	*f2;

	// if (philo_data->name % 2)
	// {
	// 	f1 = philo_data->right_fork;
	// 	f2 = philo_data->left_fork;
	// }
	// else
	// {
		f1 = philo_data->left_fork;
		f2 = philo_data->right_fork;
	// }
	if (philo_take_fork(f1) < 0)
		return (-1);
	if (philo_print(philo_data, TAKEFORK_STR, 1))
		return (-1);
	if (philo_take_fork(f2) < 0)
		return (-1);
	if (philo_print(philo_data, TAKEFORK_STR, 1))
		return (-1);
	if (philo_update_eat_stats(philo_data))
		return (-1);
	if (philo_print(philo_data, EATING_STR, 1) < 0)
		return (-1);
	if (ft_sleep_ms(philo_data->shared->time_to_eat, \
		(t_stopf)philo_sim_stop, philo_data))
		return (-1);
	if (philo_drop_fork(philo_data->left_fork) < 0 || \
		philo_drop_fork(philo_data->right_fork) < 0)
		return (-1);
	// if (philo_leave_table(philo_data) < 0)
	// 	return (-1);
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo			*philo_data;

	philo_data = arg;
	if (philo_data->name % 2)
		usleep(250);
	while (1)
	{
		if (philo_thinking(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_eat(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_sleep(philo_data) < 0)
			return (philo_set_error(philo_data));
		if (philo_sim_stop(philo_data))
			return (NULL);
		if (philo_data->shared->time_to_eat == 0 && \
			philo_data->shared->time_to_sleep)
		{
			if (usleep(PHILO_YIELD_US))
				return (philo_set_error);
		}
	}
}
