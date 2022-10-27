#include <philo.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mutex.h"
#include <stdio.h>
#include <unistd.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	ret = malloc(count * size);
	if (ret == NULL)
		return (NULL);
	memset(ret, 0, count * size);
	return (ret);
}

int	ft_gettime_ms(unsigned int *millis)
{
	struct timeval			current_time;

	if (gettimeofday(&current_time, NULL) < 0)
		return (-1);
	*millis = ((unsigned int)current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	return (0);
}

int	ft_sleep_ms(unsigned int ms, int (*stop_sleeping_f)(void *), void *arg)
{
	unsigned int	start_ms;
	unsigned int	current_ms;
	int				stop_sleeping;

	if (ft_gettime_ms(&start_ms) < 0)
		return (-1);
	stop_sleeping = 0;
	while (1)
	{
		if (stop_sleeping_f)
			stop_sleeping = stop_sleeping_f(arg);
		if (stop_sleeping < 0)
			return (-1);
		if (stop_sleeping)
			break ;
		if (usleep(PHILO_YIELD_US) < 0)
			return (-1);
		if (ft_gettime_ms(&current_ms))
			return (-1);
		if (current_ms - start_ms >= ms)
			break ;
	}
	return (0);
}
