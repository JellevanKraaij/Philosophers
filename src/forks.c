#include "philo.h"
#include "mutex.h"

static int	forks_create_mutex(t_fork	*forks, unsigned int count)
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

int	forks_create(t_fork	**forks, unsigned int count)
{
	unsigned int	i;

	*forks = ft_calloc(count, sizeof(t_fork));
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

void	forks_destroy(t_fork *forks, unsigned int count)
{
	unsigned int	i;

	i = 0;
	while (i < count)
	{
		mutex_destroy(&forks[i].mutex);
		i++;
	}
}
