/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:34:44 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 16:36:35 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
			printf("%d\t%d %s (%d times)\n", time_passed, philo->id,
				activity(state), philo->meal);
		else
			printf("%d\t%d %s\n", time_passed, philo->id, activity(state));
		if (state == 5)
			rules->write_off = 1;
	}
	pthread_mutex_unlock(&rules->global_mutex);
	return (0);
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
	return ((tv.tv_sec - tv_beg.tv_sec) * 1000
		+ ((tv.tv_usec - tv_beg.tv_usec) / 1000));
}
