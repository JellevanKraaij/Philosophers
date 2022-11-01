#include "philo.h"
#include "mutex.h"

/**
 * @brief create mutex for each fork
 *
 * @param forks	forks that need mutex
 * @param count amount of forks
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
static int	forks_create_mutex(t_fork *forks, unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < count)
	{
		if (mutex_create(&forks[i].mutex) < 0)
			return (-1);
		i++;
	}
	return (0);
}

/**
 * @brief create forks
 *
 * @param[out]	forks	destination
 * @param[in]	count	amount of forks
 * @return int	int 0 in case of SUCCESS, -1 if ERROR
 */
int	forks_create(t_fork	**forks, unsigned int count)
{
	unsigned int	i;

	*forks = ft_calloc(count + 1, sizeof(t_fork));
	if (*forks == NULL)
		return (-1);
	i = 0;
	while (i < count)
	{
		mutex_init(&(*forks)[i].mutex);
		i++;
	}
	if (forks_create_mutex(*forks, count) < 0)
	{
		forks_destroy(*forks, count);
		return (-1);
	}
	return (0);
}

/**
 * @brief destroy forks
 *
 * @param[in]	forks source
 * @param[in]	count amount of forks
 * @return int	int 0 in case of SUCCESS, -1 if ERROR
 */
int	forks_destroy(t_fork *forks, unsigned int count)
{
	unsigned int	i;
	int				ret;

	i = 0;
	ret = 0;
	while (i < count)
	{
		if (mutex_destroy(&forks[i].mutex) < 0)
			ret = -1;
		i++;
	}
	return (-1);
}
