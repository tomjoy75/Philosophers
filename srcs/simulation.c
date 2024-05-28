/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:02:10 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/28 16:33:50 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
static int	check_priority(t_philo *philo, t_rules *rules)
{
	return (philo->id == rules->priority);
}
*/
static int	lock_even_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(&(philo->l_fork));
	while (philo->l_locked /*|| !check_priority(philo, rules)*/)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
	}
	philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	// Take a fork
	if (timestamp(*philo, rules, 1))
		return (1);
	pthread_mutex_lock(philo->r_fork);
	while (*philo->r_locked /*|| !check_priority(philo, rules)*/)
	{
		pthread_mutex_unlock(philo->r_fork);
		usleep(100);
		pthread_mutex_lock(philo->r_fork);
	}
	*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	return (0);
}

static int	lock_odd_philo(t_philo *philo, t_rules *rules)
{
	pthread_mutex_lock(philo->r_fork);
	while (*philo->r_locked /*|| !check_priority(philo, rules)*/)
	{
		pthread_mutex_unlock(philo->r_fork);
		usleep(100);
		pthread_mutex_lock(philo->r_fork);
	}
	*philo->r_locked = 1;
	pthread_mutex_unlock(philo->r_fork);
	// Take a fork
	if (timestamp(*philo, rules, 1))
		return (1) ;
	pthread_mutex_lock(&(philo->l_fork));
	while (philo->l_locked /*|| !check_priority(philo, rules)*/)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
	}
	philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	return (0);
}

static void	*routine(void *args)
{
	t_philo	*philo = ((t_args *)args)->philo;
	t_rules	*rules = ((t_args *)args)->rules;
	if (philo->id % 2 == 0)
		usleep(40 * rules->nb_philo);
//	while (rules->nb_eating > 0)
	while (philo->meal < rules->nb_of_meals || rules->nb_of_meals == -2) 
	{
		if (philo->id % 2 == 0)
		{
			if (lock_even_philo(philo, rules))
				return(rules->error_flag = 1, NULL);
		}
		else
		{
			if (lock_odd_philo(philo, rules))
				return(rules->error_flag = 1, NULL);
		}
		// Eating 
		if (gettimeofday(&(philo->last_eat), NULL))
			return(rules->error_flag = 1, NULL);
		if (timestamp(*philo, rules, 2))
			return(rules->error_flag = 1, NULL);
		usleep(rules->time_to_eat * 1000);
		pthread_mutex_lock(&(philo->l_fork));
		philo->l_locked = 0;
		pthread_mutex_unlock(&(philo->l_fork));
		pthread_mutex_lock(philo->r_fork);
		*philo->r_locked = 0;
		pthread_mutex_unlock(philo->r_fork);
		// Sleeping
		if (timestamp(*philo, rules, 3))
			return(rules->error_flag = 1, NULL);
		if (rules->nb_of_meals != -2)
		{
			philo->meal++;
			if (philo->meal >= rules->nb_of_meals)
			{
//				printf("!!!philo %d has finished the %d meal!!!\n", philo->id, philo->meal);
				rules->nb_eating--;
				break ;
			}
		}
		usleep(rules->time_to_sleep * 1000);
		// Thinking
		if (timestamp(*philo, rules, 4))
			return(rules->error_flag = 1, NULL);
		if (rules->nb_philo % 2 && rules->time_to_eat > rules->time_to_sleep)
			usleep((rules->time_to_eat - rules->time_to_sleep) * 1000);
		else if (rules->nb_philo % 2 && rules->time_to_eat == rules->time_to_sleep)
		{
//			printf ("Philo %d Activate thinking time : %d\n", philo->id, (rules->time_to_die - rules->time_to_eat - rules->time_to_sleep) / 3);
			usleep ((rules->time_to_die - rules->time_to_eat - rules->time_to_sleep) * 1000 / 2);//TODO:Cas retour negatif
		}
		while (*philo->r_locked || philo->l_locked)
			usleep(100);
	}
	free(args);
	return (NULL);
}
/*
int	is_prior(t_philo *philo, t_rules *rules)
{
	
}
*/
/*
void	*dead(void *args)
{
	t_rules			*rules = (t_rules *)args;
	t_philo			*philos = rules->philos;
	int				frequence = 100;
	int				time_passed;
	struct timeval	tv_act;

//	while (rules->nb_eating > 0)
	while (philo->meal < rules->nb_of_meals || rules->nb_of_meals == -2) 
	{
		if (gettimeofday(&tv_act, NULL))
			return (NULL);
		time_passed = (((tv_act.tv_sec - philo->last_eat.tv_sec) * 1000)
				+ ((tv_act.tv_usec - philo->last_eat.tv_usec) / 1000));
		if (time_passed > rules->time_to_die)
		{
			// Died
			timestamp(*philo, rules, 5);
			rules->nb_of_meals = 0;
		}
		// TODO: Gestion de priorite
		// Pour les philos impairs, assigner une priorite
		// 1 . Condition
		// 2 . Initialisation (last_eat_max & priority)
		// 3 . Mettre dans les conditions de routine
// 		else if (time_passed > rules->last_eat_max)
// 		{
// 			rules->last_eat_max = 
// 			rules->priority = philo->id;
// 		}
		usleep (frequence);
	}
	return (NULL);
}*/

void	*monitor(void *args)
{
	t_rules			*rules = (t_rules *)args;
	// t_philo			*philos = rules->philos;
	// int				frequence = 100;
	// int				time_passed;
	// struct timeval	tv_act;
	// int				i;

	// Pour chaques philosophe :
	// 1 . Verifier si il a mange tous les repas
	// 2 . Verifier si il est mort
	//i = 0;
/*	while (1)
	{
		
	}*/
	printf ("Monitoring %d philos\n", rules->nb_philo);
	return (NULL);
}

// Create 2 threads by philosoopher, 1 for the simulation, and 1 for stopping it(die or everybody have eaten)
int	start_simulation(t_philo *philos, t_rules *rules)
{
	int	i;
	t_args	*args = malloc(sizeof(t_args));

	i = 0;
	while (i < rules->nb_philo)
	{
//		if (philos[i].id % 2 == 1)
//			usleep(100);
		args = malloc(sizeof(t_args));
		args->philo = &(philos[i]);
		args->rules = rules;
		if (pthread_create(&(philos[i].t_id), NULL, &routine, (void *)args))
			return (free(args), 1);
		i++;
	}
	if (pthread_create(&(rules->t_dead), NULL, &monitor, (void *)rules))
		return (free(args), 1);
	i = 0;
	while (i < rules->nb_philo)
	{
		if (pthread_join(philos[i].t_id, NULL))
			return (free(args), 1);
		i++;
	}
	if (pthread_join(philos[i].t_id, NULL))
		return (free(args), 1);
	if (rules->error_flag)
		return (free(args), 1);
	return (free(args), 0);
}
