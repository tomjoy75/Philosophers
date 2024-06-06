/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:02:10 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 18:13:24 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
static int	check_priority(t_philo *philo, t_rules *rules)
{
	return (philo->id == rules->priority);
}
*/
static int	mtx_check_ending(t_rules *rules)
{
	pthread_mutex_lock(&(rules->global_mutex));
	if (rules->simulation_finished)
	{
		pthread_mutex_unlock(&(rules->global_mutex));
		return (1);
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	return (0);
}

static int	several_phases(t_philo *philo, t_rules *rules, int *break_flag)
{
	if (philo->id % 2 == 0)
	{
		if (lock_even_philo(philo, rules))
			return (1);
	}
	else
	{
		if (lock_odd_philo(philo, rules))
			return (1);
	}
	if (mtx_check_ending(rules))
		return (0);
	if (!eating_time(philo, rules))
		return (1);
	if (mtx_check_ending(rules) || rules->nb_philo == 1)
		return (0);
	if (!sleeping_time(philo, rules, break_flag))
		return (1);
	return (2);
}

static int	run_routine(t_philo *philo, t_rules *rules, int *break_flag)
{
	int		return_value;

	pthread_mutex_lock(&(rules->global_mutex));
	while (!rules->simulation_finished && !rules->error_flag && (philo->meal
			< rules->nb_of_meals || rules->nb_of_meals == -2))
	{		
		pthread_mutex_unlock(&(rules->global_mutex));
		return_value = several_phases(philo, rules, break_flag);
		if (return_value == 0 || return_value == 1)
			return (return_value);
		pthread_mutex_lock(&(rules->global_mutex));
		if (*break_flag || rules->simulation_finished)
		{
			pthread_mutex_unlock(&(rules->global_mutex));
			return (0);
		}	
		pthread_mutex_unlock(&(rules->global_mutex));
		thinking_time(philo, rules);
		pthread_mutex_lock(&(rules->global_mutex));
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	return (0);
}

static void	*routine(void *args)
{
	t_philo	*philo;
	t_rules	*rules;
	int		break_flag;

	break_flag = 0;
	philo = ((t_args *)args)->philo;
	rules = ((t_args *)args)->rules;
	while (get_current_time(rules->tv_beg) < 0)
		usleep(100);
	if (philo->id % 2 == 0)
		usleep(40 * rules->nb_philo);
	if (run_routine(philo, rules, &break_flag))
	{
		pthread_mutex_lock(&(rules->global_mutex));
		rules->error_flag = 1;
		pthread_mutex_unlock(&(rules->global_mutex));
	}
	free(args);
	return (NULL);
}

// Create 1 threads by philosoopher for the simulation,
// and 1 for stopping it(die or everybody have eaten)
int	start_simulation(t_philo *philos, t_rules *rules)
{
	int		i;
	t_args	*args;

	i = 0;
	while (i < rules->nb_philo)
	{
		args = malloc(sizeof(t_args));
		if (!args)
			return (1);
		args->philo = &(philos[i]);
		args->rules = rules;
		if (pthread_create(&(philos[i].t_id), NULL, &routine, (void *)args))
			return (free(args), 1);
		i++;
	}
	if (pthread_create(&(rules->t_monitor), NULL, &monitor, (void *)rules))
		return (1);
	if (pthread_join(rules->t_monitor, NULL))
		return (1);
	i = -1;
	while (++i < rules->nb_philo)
		if (pthread_join(philos[i].t_id, NULL))
			return (1);
	return (rules->error_flag);
}
