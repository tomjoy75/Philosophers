/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:02:10 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/22 16:10:22 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
static int	lock_even_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&(philo->l_fork));
	if (philo->l_locked)
		while (philo->l_locked)
			usleep(100);
	else
		philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	if (timestamp(*philo, *rules, 1))
		return (1);
	pthread_mutex_lock(philo->r_fork);
	if (*philo->r_locked)
		while (*philo->r_locked)
			usleep(100);
	else
		*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}

static int	lock_odd_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(philo->r_fork);
	if (*philo->r_locked)
		while (*philo->r_locked)
			usleep(100);
	else
		*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	if (timestamp(*philo, *rules, 1))
		return (1) ;
	pthread_mutex_lock(&(philo->l_fork));
	if (philo->l_locked)
		while (philo->l_locked)
			usleep(100);
	else
		philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	return (0);
}

static void	*routine(void *args)
{
	t_philo	*philo = ((t_args *)args)->philo;
	t_rules	*rules = ((t_args *)args)->rules;

	if (philo->id % 2 == 1)
		usleep(100);
	while (!rules->finished)
	{
		if (philo->id % 2 == 0)
		{
			if (lock_even_philo(philo, rules))
				break ;
/*			pthread_mutex_lock(&(philo->l_fork));
			if (philo->l_locked)
				while (philo->l_locked)
					usleep(100);
			else
				philo->l_locked = 1;
			pthread_mutex_unlock(&(philo->l_fork));
			if (timestamp(*philo, *rules, 1))
				break ;
			pthread_mutex_lock(philo->r_fork);
			if (*philo->r_locked)
				while (*philo->r_locked)
					usleep(100);
			else
				*philo->r_locked = 1;
			pthread_mutex_unlock(philo->r_fork);*/
		}
		else
		{
			if (lock_odd_philo(philo, rules))
				break ;
/*			pthread_mutex_lock(philo->r_fork);
			if (*philo->r_locked)
				while (*philo->r_locked)
					usleep(100);
			else
				*philo->r_locked = 1;
			pthread_mutex_unlock(philo->r_fork);
			if (timestamp(*philo, *rules, 1))
				break ;
			pthread_mutex_lock(&(philo->l_fork));
			if (philo->l_locked)
				while (philo->l_locked)
					usleep(100);
			else
				philo->l_locked = 1;
			pthread_mutex_unlock(&(philo->l_fork));*/
		}
		if (timestamp(*philo, *rules, 2))
			break ;
		if (gettimeofday(&(philo->last_eat), NULL))
			return (NULL);
		usleep(rules->time_to_eat * 1000);
		philo->l_locked = 0;
		*philo->r_locked = 0;
		if (timestamp(*philo, *rules, 3))
			break ;
		usleep(rules->time_to_sleep * 1000);
		if (timestamp(*philo, *rules, 4))
			break ;
		usleep(100);
	}
	free(args);
}

void	*dead(void *args)
{
	t_philo			*philo = ((t_args *)args)->philo;
	t_rules			*rules = ((t_args *)args)->rules;
	int				frequence = 1000;
	int				time_passed;
	struct timeval	tv_act;

	while (!rules->finished)
	{
		if (gettimeofday(&tv_act, NULL))
			return (NULL);
		time_passed = (((tv_act.tv_sec - philo->last_eat.tv_sec) * 1000)
				+ ((tv_act.tv_usec - philo->last_eat.tv_usec) / 1000));
		if (time_passed > rules->time_to_die)
		{
			timestamp(*philo, *rules, 5);
			rules->finished = 1;
		}
		usleep (frequence);
	}
}

// Create 2 threads by philosoopher, 1 for the simulation, and 1 for stopping it(die or everybody have eaten)
int	start_simulation(t_philo *philos, t_rules *rules)
{
	int	i;
	t_args	*args;

	i = 0;
	while (i < rules->nb_philo)
	{
		if (philos[i].id % 2 == 1)
			usleep(100);
		args = malloc(sizeof(t_args));
		args->philo = &(philos[i]);
		args->rules = rules;
		if (pthread_create(&(philos[i].t_id), NULL, &routine, (void *)args))
			return (free(args), 1);
		if (pthread_create(&(philos[i].t_id), NULL, &dead, (void *)args))
			return (free(args), 1);
		i++;
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_join(philos[i].t_id, NULL))
			return (free(args), 1);
		i++;
	}
	return (free(args), 0);
}
