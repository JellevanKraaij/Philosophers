#include <philo.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "mutex.h"
#include <stdio.h>
#include <unistd.h>

/**
 * @brief allocate and binary zero
 *
 * @param[in] count element count
 * @param[in] size element size
 * @return void* pointer to allocated memory or NULL in case of error
 */
void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	ret = malloc(count * size);
	if (ret == NULL)
		return (NULL);
	memset(ret, 0, count * size);
	return (ret);
}

/**
 * @brief get system time in milli seconds
 *
 * @param[out] millis
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	ft_gettime_ms(unsigned int *millis)
{
	struct timeval			current_time;

	if (gettimeofday(&current_time, NULL) < 0)
		return (-1);
	*millis = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (0);
}

/**
 * @brief sleep milliseconds
 *
 * @param[in] ms milliseconds to sleep
 * @param[in] stop_sleep_f exit sleeping if function return 1
 * @param[in] arg argument for stop_sleep_f functon
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	ft_sleep_ms(unsigned int ms, int (*stop_sleep_f)(void *), void *arg)
{
	unsigned int	start_ms;
	unsigned int	current_ms;
	int				stop_sleeping;

	if (ft_gettime_ms(&start_ms) < 0)
		return (-1);
	stop_sleeping = 0;
	while (1)
	{
		if (ft_gettime_ms(&current_ms))
			return (-1);
		if (current_ms - start_ms >= ms)
			break ;
		if (stop_sleep_f)
			stop_sleeping = stop_sleep_f(arg);
		if (stop_sleeping < 0)
			return (-1);
		if (stop_sleeping)
			break ;
		if (usleep(PHILO_YIELD_US) < 0)
			return (-1);
	}
	return (0);
}
