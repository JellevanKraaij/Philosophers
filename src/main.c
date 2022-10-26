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

int	main(int argc, char **argv)
{
	t_params	params;

	memset(&params, 0, sizeof(params));
	if (parse_params(argc, argv, &params) < 0)
		return (1);
	if (params_init(&params) < 0)
	{
		params_destroy(&params);
		printf("system error\n");
		return (1);
	}
	run_simulation(&params);
	params_destroy(&params);
}
