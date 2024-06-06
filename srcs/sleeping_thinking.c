/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sleeping_thinking.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:46:30 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 18:11:11 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*sleeping_time(t_philo *philo, t_rules *rules, int *break_flag)
{
	pthread_mutex_lock(&(rules->global_mutex));
	if (rules->nb_of_meals != -2)
	{
		philo->meal++;
		if (philo->meal >= rules->nb_of_meals)
		{
			rules->nb_eating--;
			*break_flag = 1;
			pthread_mutex_unlock(&(rules->global_mutex));
			return ((void *)1);
		}
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	if (timestamp(philo, rules, 3))
		return (NULL);
	if (rules->time_to_sleep <= rules->time_to_die - rules->time_to_eat)
		usleep(rules->time_to_sleep * 1000);
	else
		usleep((rules->time_to_die - rules->time_to_eat + 10) * 1000);
	return ((void *)1);
}

static void	even_time_thinking(t_philo *philo)
{
	pthread_mutex_lock(&(philo->l_fork));
	pthread_mutex_lock(philo->r_fork);
	while (*philo->r_locked || philo->l_locked)
	{
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
		pthread_mutex_lock(philo->r_fork);
	}
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(&(philo->l_fork));
}

static void	odd_time_thinking(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	pthread_mutex_lock(&(philo->l_fork));
	while (*philo->r_locked || philo->l_locked)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		pthread_mutex_unlock(philo->r_fork);
		usleep(100);
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(&(philo->l_fork));
	}
	pthread_mutex_unlock(&(philo->l_fork));
	pthread_mutex_unlock(philo->r_fork);
}

void	thinking_time(t_philo *philo, t_rules *rules)
{
	if (rules->nb_philo == 1)
		return ;
	if (timestamp(philo, rules, 4))
	{
		rules->error_flag = 1;
		return ;
	}
	if (rules->nb_philo % 2 && rules->time_to_eat >= rules->time_to_sleep)
		usleep ((rules->time_to_die - rules->time_to_eat - rules->time_to_sleep)
			* 1000 / 2);
	if (philo->id % 2 == 0)
		even_time_thinking(philo);
	else
		odd_time_thinking(philo);
}
