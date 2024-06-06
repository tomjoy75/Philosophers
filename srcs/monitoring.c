/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:41:46 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 18:05:35 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_rules *rules, t_philo *philos, int i)
{
	struct timeval	tv_act;

	if (rules->nb_of_meals == -2 || philos[i].meal < rules->nb_of_meals)
	{	
		pthread_mutex_unlock(&(rules->global_mutex));
		if (gettimeofday(&tv_act, NULL))
			return (1);
		pthread_mutex_lock(&(rules->global_mutex));
		if (time_passed(tv_act, philos[i].last_eat) >= rules->time_to_die)
		{
			rules->simulation_finished++;
			pthread_mutex_unlock(&(rules->global_mutex));
			if (timestamp(philos + i, rules, 5))
				return (1);
			return (1);
		}
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	return (0);
}

void	*monitor(void *args)
{
	t_rules			*rules;
	t_philo			*philos;
	int				i;

	i = 0;
	rules = (t_rules *)args;
	philos = rules->philos;
	while (get_current_time(rules->tv_beg) < 0)
		usleep(100);
	pthread_mutex_lock(&(rules->global_mutex));
	while (rules->nb_eating > 0 || rules->nb_of_meals == -2)
	{
		if (is_dead(rules, philos, i))
			return (NULL);
		i++;
		if (i == rules->nb_philo)
			i = 0;
		usleep (200 / rules->nb_philo);
		pthread_mutex_lock(&(rules->global_mutex));
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	return (NULL);
}
