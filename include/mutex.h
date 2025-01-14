/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mutex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra <jvan-kra@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/11/03 15:02:01 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/11/03 15:02:01 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTEX_H
# define MUTEX_H

# include <pthread.h>

typedef struct s_mutex
{
	pthread_mutex_t	mutex;
	int				initialized;
}	t_mutex;

void	mutex_init(t_mutex *mutex);
int		mutex_create(t_mutex *mutex);
int		mutex_destroy(t_mutex *mutex);
int		mutex_lock(t_mutex *mutex);
int		mutex_unlock(t_mutex *mutex);

#endif
