/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 10:15:46 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/05 17:13:32 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_exit(const char *error)
{
	printf(RED"Error :\n\t"ITALIC"%s\n"RESET, error);
	return (1);
}

void	*error_exit_null(const char *error)
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

static char	*activity(int state)
{
	if (state == 1)
		return ("has taken a fork");
	else if (state == 2)
		return ("is eating");
	else if (state == 3)
		return ("is sleeping");
	else if (state == 4)
		return ("is thinking");
	else if (state == 5)
		return ("died");
	else if (state == 6)
		return ("___check dying___");
	else if (state == 7)
		return ("has eaten enough");
	return ("");
}

int	time_passed(struct timeval tv_end, struct timeval tv_beg)
{
	return (((tv_end.tv_sec - tv_beg.tv_sec) * 1000)
			+ ((tv_end.tv_usec - tv_beg.tv_usec) / 1000));
}

int	get_current_time(struct timeval tv_beg)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (1);
	return ((tv.tv_sec - tv_beg.tv_sec) * 1000	+ ((tv.tv_usec - tv_beg.tv_usec) / 1000));
}



void	mtx_printf_noarg(t_rules *rules, char *str)
{
	pthread_mutex_lock(&rules->global_mutex);
	printf("%s\n", str);
	pthread_mutex_unlock(&rules->global_mutex);
}

void	mtx_printf_arg(t_rules *rules, char *str, int i)
{
	pthread_mutex_lock(&rules->global_mutex);
	printf("%s\t[%i]\n", str, i);
	pthread_mutex_unlock(&rules->global_mutex);
}


int	timestamp(t_philo *philo, t_rules *rules, int state)
{
	struct timeval	tv_act;
	int				time_passed;

	if (gettimeofday(&tv_act, NULL))
		return (1);
	time_passed = (((tv_act.tv_sec - rules->tv_beg.tv_sec) * 1000)
			+ ((tv_act.tv_usec - rules->tv_beg.tv_usec) / 1000));
	pthread_mutex_lock(&rules->global_mutex);
	if (!rules->write_off)
	{
		if (state == 7)
			printf("%d\t%d %s (%d times)\n", time_passed, philo->id, activity(state), philo->meal);
		else
			printf("%d\t%d %s\n", time_passed, philo->id, activity(state));
		if (state == 5)
			rules->write_off = 1;
	}
	pthread_mutex_unlock(&rules->global_mutex);
	return (0);
}
