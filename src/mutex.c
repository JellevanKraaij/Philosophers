#include "mutex.h"
#include <pthread.h>
#include <limits.h>
/**
 * @brief initialize mutex struct
 *
 * @param mutex
 */
void	mutex_init(t_mutex *mutex)
{
	mutex->initialized = 0;
}

/**
 * @brief create mutex
 * @attention init mutex first
 *
 * @param mutex
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	mutex_create(t_mutex *mutex)
{
	if (mutex->initialized)
		return (-1);
	if (pthread_mutex_init(&mutex->mutex, NULL) < 0)
		return (-1);
	mutex->initialized = 1;
	return (0);
}

/**
 * @brief destroys mutex
 * @attention init mutex first
 *
 * @param mutex
 * @return int 0 in case of SUCCESS, -1 if ERROR
 */
int	mutex_destroy(t_mutex *mutex)
{
	if (!mutex->initialized)
		return (-1);
	if (pthread_mutex_destroy(&mutex->mutex))
		return (-1);
	mutex->initialized = 0;
	return (0);
}

/**
 * @brief locks mutex
 * @attention init mutex first
 *
 * @param mutex
 * @return int int 0 in case of SUCCESS, -1 if ERROR
 */
int	mutex_lock(t_mutex *mutex)
{
	if (!mutex->initialized)
		return (-1);
	return (pthread_mutex_lock(&mutex->mutex));
}

/**
 * @brief unlocks mutex
 * @attention init mutex first
 *
 * @param mutex
 * @return int int 0 in case of SUCCESS, -1 if ERROR
 */
int	mutex_unlock(t_mutex *mutex)
{
	if (!mutex->initialized)
		return (-1);
	return (pthread_mutex_unlock(&mutex->mutex));
}
