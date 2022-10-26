#include "philo.h"
#include <stdio.h>

static int	parse_numbers(char **src, int count, unsigned int **dst)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (ft_atoui_safe(src[i], dst[i]) < 0)
			return (-1);
		i++;
	}
	return (0);
}

int	parse_params(int argc, char **argv,	t_params *params)
{
	const unsigned int	*numbers[] = \
	{
		&params->number_philos, &params->time_to_die,
		&params->time_to_eat, &params->time_to_sleep,
		&params->times_must_eat
	};

	if (argc < 5 || argc > 6 || \
		parse_numbers(argv + 1, argc - 1, (unsigned int **)numbers) < 0)
	{
		printf("fdf: incorrect parameter\n");
		printf("usage: ./fdf %s %s %s %s [%s]\n", \
			"number_of_philosophers", "time_to_die", \
			"time_to_eat", "time_to_sleep", \
			"number_of_times_each_philosopher_must_eat");
		return (-1);
	}
	return (0);
}
