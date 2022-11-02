/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/03/24 17:00:21 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/03/24 17:00:21 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "philo.h"
#include <stdlib.h>

int	print_error_cleanup(t_params *params)
{
	if (params)
		params_destroy(params);
	printf("system error\n");
	return (1);
}

int	main(int argc, char **argv)
{
	t_params	params;
	int			result;

	memset(&params, 0, sizeof(params));
	result = params_init(&params, argc, argv);
	if (result < 0)
		return (print_error_cleanup(NULL));
	if (result > 0)
		return (-1);
	if (start_simulation(&params))
		return (print_error_cleanup(&params));
	if (monitor_simulation(&params) < 0)
	{
		end_threads(&params);
		return (print_error_cleanup(&params));
	}
	if (end_simulation(&params) < 0)
		return (print_error_cleanup(&params));
	if (params.sim_exit_error)
		return (print_error_cleanup(&params));
	if (params_destroy(&params) < 0)
		return (print_error_cleanup(NULL));
	return (EXIT_SUCCESS);
}
