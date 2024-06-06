/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eating.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 18:12:14 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 18:15:59 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	lock_even_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&(philo->l_fork));
	while (philo->l_locked)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
	}
	philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	if (timestamp(philo, rules, 1))
		return (1);
	pthread_mutex_lock(philo->r_fork);
	while (*philo->r_locked)
	{
		pthread_mutex_unlock(philo->r_fork);
		usleep(100);
		pthread_mutex_lock(philo->r_fork);
	}
	*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}

int	lock_odd_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(philo->r_fork);
	while (*philo->r_locked)
	{
		pthread_mutex_unlock(philo->r_fork);
		usleep(100);
		pthread_mutex_lock(philo->r_fork);
	}
	*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	if (timestamp(philo, rules, 1))
		return (1);
	pthread_mutex_lock(&(philo->l_fork));
	while (rules->nb_philo != 1 && philo->l_locked)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
	}
	philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	return (0);
}

static void	free_locks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&(philo->l_fork));
		pthread_mutex_lock(philo->r_fork);
		philo->l_locked = 0;
		*philo->r_locked = 0;
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(&(philo->l_fork));
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(&(philo->l_fork));
		*philo->r_locked = 0;
		philo->l_locked = 0;
		pthread_mutex_unlock(&(philo->l_fork));
		pthread_mutex_unlock(philo->r_fork);
	}
}

void	*eating_time(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&(rules->global_mutex));
	if (gettimeofday(&(philo->last_eat), NULL))
	{
		pthread_mutex_unlock(&(rules->global_mutex));
		return (NULL);
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	if (timestamp(philo, rules, 2))
	{
		return (NULL);
	}
	if (rules->time_to_eat <= rules->time_to_die)
		usleep(rules->time_to_eat * 1000);
	else
		usleep((rules->time_to_die + 10) * 1000);
	if (rules->nb_philo != 1)
		free_locks(philo);
	return ((void *)1);
}
