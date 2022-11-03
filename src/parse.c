/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parse.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:02:19 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:02:19 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

/**
 * @brief parse array of strings to array of numbers
 *
 * @param[in] src array of strings
 * @param[in] count amount of strings
 * @param[out] dst array of numbers
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
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

/**
 * @brief parse program parameters
 *
 * @param argc program argument count
 * @param argv program arguments
 * @param params parameter destination
 * @return int 0 in case of SUCCESS, -1 if ERROR, 1 if USER_ERROR
 */
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
		if (printf("philosophers: incorrect parameter\n") < 0)
			return (-1);
		if (printf("usage: ./philo %s %s %s %s [%s]\n", \
			"number_of_philosophers", "time_to_die", \
			"time_to_eat", "time_to_sleep", \
			"number_of_times_each_philosopher_must_eat") < 0)
			return (-1);
		return (1);
	}
	if (argc == 6)
		params->times_must_eat_enabled = 1;
	return (0);
}
