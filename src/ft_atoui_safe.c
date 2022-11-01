/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoui_safe.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/25 19:58:53 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/10/25 19:58:53 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <limits.h>

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief protected string to unsigned interger converter
 * protected against overflow & non numeric characters
 * @param str
 * @param num
 * @return int
 */
int	ft_atoui_safe(const char *str, unsigned int *num)
{
	long	ret;
	int		len;

	ret = 0;
	len = 0;
	while (ft_isdigit(*str))
	{
		ret = (ret * 10) + (*str - '0');
		if (ret > (long)UINT_MAX)
			return (-1);
		str++;
		len++;
	}
	if (*str != '\0')
		return (-1);
	*num = ret;
	return (len);
}
