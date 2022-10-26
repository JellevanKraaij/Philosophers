#include <philo.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <mutex.h>
#include <stdio.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ret;

	ret = malloc(count * size);
	if (ret == NULL)
		return (NULL);
	memset(ret, 0, count * size);
	return (ret);
}

int	gettime_ms(unsigned int *millis)
{
	struct timeval			current_time;

	if (gettimeofday(&current_time, NULL) < 0)
		return (-1);
	*millis = current_time.tv_sec + (current_time.tv_usec / 1000);
	return (0);
}

int	print_time_protected(unsigned int time, unsigned int philo_name, \
	char *str, t_mutex *print_mutex)
{
	if (mutex_lock(print_mutex) < 0)
		return (-1);
	if (printf("%u %u %s\n", time, philo_name, str) < 0)
	{
		mutex_unlock(print_mutex);
		return (-1);
	}
	if (mutex_unlock(print_mutex) < 0)
		return (-1);
	return (0);
}
