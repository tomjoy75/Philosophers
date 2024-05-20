/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:15:05 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/20 18:36:55 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
//# include "philo.h"

// Philosopher simplifie
// 2 Philosophe...Quand phi 1 mange, l'autre ne mange pas
typedef struct s_philo
{
	int				id;
	pthread_t		t_id;
	pthread_mutex_t	l_fork;
	int				l_locked;
	pthread_mutex_t	*r_fork;
	int				*r_locked;
	int				state;
	struct timeval	last_eat;
}				t_philo;

typedef struct s_rules
{
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	struct timeval	tv_beg;
	int				finished;
}				t_rules;

typedef struct s_args
{
	t_philo	*philo;
	t_rules	*rules;
}				t_args;

static int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (2048);
	else
		return (0);
}

int	ft_atoi(const char *nptr)
{
	int	i;
	int	n;
	int	sign;

	i = 0;
	n = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-')
		sign = -1;
	if (nptr[i] == '-' || nptr[i] == '+')
		i++;
	if (nptr[i] < '0' || nptr[i] > '9')
		return (0);
	while (ft_isdigit(nptr[i]))
	{
		n *= 10;
		n += nptr[i] - 48;
		i++;
	}
	return (n * sign);
}

int	timestamp(t_philo philo, t_rules rules, int state)
{
	struct timeval	tv_act;
	int				time_passed;

	if (rules.finished)
		return (1);
	usleep(100);
	if (gettimeofday(&tv_act, NULL))
		return (1);
	time_passed = (((tv_act.tv_sec - rules.tv_beg.tv_sec) * 1000)
			+ ((tv_act.tv_usec - rules.tv_beg.tv_usec) / 1000));
	if (state == 1)
		printf("%d\t%d has taken a fork\n", time_passed, philo.id);
	else if (state == 2)
		printf("%d\t%d is eating\n", time_passed, philo.id);
	else if (state == 3)
		printf("%d\t%d is sleeping\n", time_passed, philo.id);
	else if (state == 4)
		printf("%d\t%d is thinking\n", time_passed, philo.id);
	else if (state == 5)
		printf("\e[31m%d\t%d died\n\e[0m", time_passed, philo.id);
	else if (state == 6)
		printf("%d\t%d ___check dying___\n", time_passed, philo.id);
	return (0);
}

void	*eat(void *args)
{
	t_philo	*philo = ((t_args *)args)->philo;
	t_rules	*rules = ((t_args *)args)->rules;

	if (philo->id % 2 == 1)
		usleep(100);
	while (!rules->finished)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&(philo->l_fork));
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
			pthread_mutex_unlock(philo->r_fork);
		}
		else
		{
			pthread_mutex_lock(philo->r_fork);
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
			pthread_mutex_unlock(&(philo->l_fork));
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

int	main(int argc, char **argv)
{
	int		nb_philo = ft_atoi(argv[1]);
	t_philo	philos[nb_philo];
	t_rules	rules;
	t_args	*args;
	int		i = 0;

	rules.nb_philo = nb_philo;
	rules.time_to_die = ft_atoi(argv[2]);
	rules.time_to_eat = ft_atoi(argv[3]);
	rules.time_to_sleep = ft_atoi(argv[4]);
	rules.finished = 0;
	if (gettimeofday(&(rules.tv_beg), NULL))
		return (1);
	while (i < nb_philo)
	{
		philos[i].id = i + 1;
		pthread_mutex_init(&(philos[i].l_fork), NULL);
		philos[i].l_locked = 0;
		if (gettimeofday(&(philos[i].last_eat), NULL))
			return (1);
		if (i == nb_philo - 1)
		{
			philos[i].r_fork = &philos[0].l_fork;
			philos[i].r_locked = &philos[0].l_locked;
		}
		else
		{
			philos[i].r_fork = &philos[i + 1].l_fork;
			philos[i].r_locked = &philos[i + 1].l_locked;
		}
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		if (philos[i].id % 2 == 1)
			usleep(100);
		args = malloc(sizeof(t_args));
		args->philo = &(philos[i]);
		args->rules = &rules;
		if (pthread_create(&(philos[i].t_id), NULL, &eat, (void *)args))
			return (1);
		if (pthread_create(&(philos[i].t_id), NULL, &dead, (void *)args))
			return (1);
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		if (pthread_join(philos[i].t_id, NULL))
			return (1);
		i++;
	}
	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_destroy(&(philos[i].l_fork));
		i++;
	}
	return (0);
}
