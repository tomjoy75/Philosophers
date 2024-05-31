/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:15:46 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/31 23:02:36 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_exit(const char *error)
{
	printf(RED"Error :\n\t"ITALIC"%s\n"RESET, error);
	return (-1);
} 

void	*error_exit_NULL(const char *error)
{
	printf(RED"Error :\n\t"ITALIC"%s\n"RESET, error);
	return (NULL);
}

void	clean_all(t_philo *philos, int n_mutex)
{
	int	i;

	i = 0;
	while (i < n_mutex)
	{
		pthread_mutex_destroy(&(philos[i].l_fork));
		i++;
	}
	free (philos);
}

int	timestamp(t_philo *philo, t_rules *rules, int state)
{
	struct timeval	tv_act;
	int				time_passed;

//	if (!rules.nb_eating)
//		return (1);
//	usleep(100);
	if (gettimeofday(&tv_act, NULL))
		return (1);
	time_passed = (((tv_act.tv_sec - rules->tv_beg.tv_sec) * 1000)
			+ ((tv_act.tv_usec - rules->tv_beg.tv_usec) / 1000));
//	printf("Attemting to lock print_mutex\n");
	pthread_mutex_lock(&rules->global_mutex);
//	printf("print_mutex locked\n");
	if (!rules->write_off)
	{
		if (state == 1)
			printf("%d\t%d has taken a fork\n", time_passed, philo->id);
		else if (state == 2)
			printf("%d\t%d is eating\n", time_passed, philo->id);
		else if (state == 3)
			printf("%d\t%d is sleeping\n", time_passed, philo->id);
		else if (state == 4)
			printf("%d\t%d is thinking\n", time_passed, philo->id);
		else if (state == 5)
	//		printf("\e[31m%d\t%d died\n\e[0m", time_passed, philo.id);
		{
			printf("%d\t%d died\n", time_passed, philo->id);
			rules->write_off = 1;
		}
		else if (state == 6)
			printf("%d\t%d ___check dying___\n", time_passed, philo->id);
	}
	
	pthread_mutex_unlock(&rules->global_mutex);
//	printf("Attemting to unlock print_mutex\n");
//	pthread_mutex_unlock(&rules->global_mutex);
//	printf("print_mutex unlocked\n");
	return (0);
}

