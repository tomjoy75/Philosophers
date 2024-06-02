/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 13:02:10 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/03 00:30:29 by joyeux           ###   ########.fr       */
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

static int	lock_odd_philo(t_philo *philo, t_rules *rules)
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
	while (philo->l_locked)
	{
		pthread_mutex_unlock(&(philo->l_fork));
		usleep(100);
		pthread_mutex_lock(&(philo->l_fork));
	}
	philo->l_locked = 1;
	pthread_mutex_unlock(&(philo->l_fork));
	return (0);
}

void	free_locks(t_philo *philo)
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
	usleep(rules->time_to_eat * 1000);
	free_locks(philo);
	return ((void *)1);
}

void	*sleeping_time(t_philo *philo, t_rules *rules, int *break_flag)
{
	pthread_mutex_lock(&(rules->global_mutex));
	if (rules->nb_of_meals != -2)
	{
		philo->meal++;
		if (philo->meal >= rules->nb_of_meals)
		{
			rules->nb_eating--; //TODO: Est ce que j'en ai besoin ?
//			printf("Philo %d has eaten %d times\n", philo->id, philo->meal);
			*break_flag = 1;
			pthread_mutex_unlock(&(rules->global_mutex));
			if (timestamp(philo, rules, 7))
				return (NULL);
			return ((void *)1);
		}
	}
	pthread_mutex_unlock(&(rules->global_mutex));
	if (timestamp(philo, rules, 3))
		return (NULL);
	usleep(rules->time_to_sleep * 1000);
	return ((void *)1);
}

void	even_time_thinking(t_philo *philo)
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

void	odd_time_thinking(t_philo *philo)
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

//TODO: Checker si on doit differencier > et = .
//TODO:Cas retour negatif sur le deuxieme usleep
void	*thinking_time(t_philo *philo, t_rules *rules)
{
	if (timestamp(philo, rules, 4))
		return (NULL);
	if (rules->nb_philo % 2 && rules->time_to_eat > rules->time_to_sleep)
		usleep((rules->time_to_eat - rules->time_to_sleep) * 1000);
	else if (rules->nb_philo % 2 && rules->time_to_eat == rules->time_to_sleep)
		usleep ((rules->time_to_die - rules->time_to_eat - rules->time_to_sleep)
			* 1000 / 2);
	if (philo->id % 2 == 0)
		even_time_thinking(philo);
	else
		odd_time_thinking(philo);
	return ((void *)1);
}

int	run_routine(t_philo *philo, t_rules *rules, int *break_flag)
{
	while (!rules->error_flag && (philo->meal < rules->nb_of_meals
			|| rules->nb_of_meals == -2))
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
		if (!eating_time(philo, rules))
			return (1);
		if (!sleeping_time(philo, rules, break_flag))
			return (1);
		if (*break_flag)
			return (0);
		if (!thinking_time(philo, rules))
			return (1);
	}
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
	if (philo->id % 2 == 0)
		usleep(40 * rules->nb_philo);
	if (run_routine(philo, rules, &break_flag))
		rules->error_flag = 1;
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
/*
void	*monitor(void *args)
{
	t_rules			*rules = (t_rules *)args;
	t_philo			*philos = rules->philos;
	// int				frequence = 100;
	// int				time_passed;
	// struct timeval	tv_act;
	// int				i;

	(void)args;
	// Pour chaques philosophe :
	// 1 . Verifier si il a mange tous les repas(necessaire??)
	// 2 . Verifier si il est mort
	//i = 0;
	while (1)
	{
		
	}
//	printf ("Monitoring %d philos\n", rules->nb_philo);
//	printf ("Monitoring philos %d\n", *philos->id);
	return (NULL);
}*/

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
//	if (pthread_create(&(rules->t_monitor), NULL, &monitor, (void *)rules))
//		return (1);
	i = -1;
	while (++i < rules->nb_philo)
		if (pthread_join(philos[i].t_id, NULL))
			return (1);
//	if (pthread_join(rules->t_monitor, NULL))
//		return (1);
	return (rules->error_flag);
}
