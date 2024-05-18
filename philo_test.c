/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:15:05 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/18 01:18:57 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

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
//	int				time_to_eat;
//	int				time_to_sleep;
//	int				time_to_die;
}				t_philo;

typedef struct s_rules
{
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	struct timeval	tv_beg;
}				t_rules;

typedef struct s_args
{
	t_philo	*philo;
	t_rules *rules;
}				t_args;

int	timestamp(t_philo philo, t_rules rules, int state)
{
	struct timeval	tv_act;
	int				time_passed;

	if (gettimeofday(&tv_act, NULL))
		return (1);
	time_passed = (((tv_act.tv_sec - rules.tv_beg.tv_sec) * 1000) + ((tv_act.tv_usec - rules.tv_beg.tv_usec) / 1000));
	if (state == 1)
		printf("%d\t%d has taken a fork\n", time_passed, philo.id);
	if (state == 2)
		printf("%d\t%d is eating\n", time_passed, philo.id);
	if (state == 3)
		printf("%d\t%d is sleeping\n", time_passed, philo.id);
	if (state == 4)
		printf("%d\t%d is thinking\n", time_passed, philo.id);
	if (state == 5)
		printf("%d\t%d died\n", time_passed, philo.id);
	if (state == 6)
		printf("%d\t%d ___check dying___\n", time_passed, philo.id);
	return (0);
}

void	*eat(void *args)
{
	t_philo	*philo = ((t_args *)args)->philo;
	t_rules	*rules = ((t_args *)args)->rules;
	if (philo->id % 2 == 1 )
		usleep(100);
	while (1)
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
			timestamp(*philo, *rules, 1);
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
			timestamp(*philo, *rules, 1);
			pthread_mutex_lock(&(philo->l_fork));
			if (philo->l_locked)
				while (philo->l_locked)
					usleep(100);
			else
				philo->l_locked = 1;
			pthread_mutex_unlock(&(philo->l_fork));
		}
/*		pthread_mutex_lock(philo->r_fork);
		timestamp(*philo, *rules, 1);
		pthread_mutex_lock(&(philo->l_fork));*/
//		philo->state == 1;
		timestamp(*philo, *rules, 2);
//		printf("Philo's [%d] eating\n", philo->id);
		usleep(rules->time_to_eat * 1000);
		philo->l_locked = 0;
		*philo->r_locked = 0;
//		pthread_mutex_unlock(&(philo->l_fork));
//		pthread_mutex_unlock(philo->r_fork);
		timestamp(*philo, *rules, 3);
//		printf("Philo's [%d] sleeping\n", philo->id);
		usleep(rules->time_to_sleep * 1000);
		timestamp(*philo, *rules, 4);
		usleep(100);
//		printf("Philo's [%d] thinking\n", philo->id);
	}
}

void	*dead(void *args)
{
	t_philo	*philo = ((t_args *)args)->philo;
	t_rules	*rules = ((t_args *)args)->rules;
	while (1)
	{
		usleep(rules->time_to_die * 1000);
		timestamp(*philo, *rules, 6);
//		printf("Check dying philo %d\n", philo->id);
	}
}

int	main()
{
	int		nb_philo = 5;
	t_philo	philos[nb_philo];
	t_rules	rules;
	t_args	*args;
	int		i = 0;

	// Creer tableau de philo
	// Remplir les rules
	rules.nb_philo = nb_philo;
	rules.time_to_die = 700;
	rules.time_to_eat = 200;
	rules.time_to_sleep = 200;
	if (gettimeofday(&(rules.tv_beg), NULL))
		return (1);
	// Remplir ces caracteristiques
	while (i < nb_philo)
	{
		philos[i].id = i + 1;
		pthread_mutex_init(&(philos[i].l_fork), NULL);
		philos[i].l_locked = 0;
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
//		if (!i)
//			error = philos[i].tv_beg
		i++;
	}
	args = malloc(sizeof(t_args) * nb_philo);
	// Envoyer programme
	i = 0;
	while (i < nb_philo)
	{
		if (philos[i].id % 2 == 1 )
				usleep(100);
		args[i].philo = &(philos[i]);
		args[i].rules = &rules;
		pthread_create(&(philos[i].t_id), NULL, &eat, (void *)&args[i]);
//		pthread_create(&(philos[i].t_id), NULL, &dead, (void *)&args[i]);
		i++;
	}
	// thread_join
	i = 0;
	while (i < nb_philo)
	{
		pthread_join(philos[i].t_id, NULL);
		i++;
	}
	// Destroy mutex
	i = 0;
	while (i < nb_philo)
	{
		pthread_mutex_destroy(&(philos[i].l_fork));
	//	pthread_de
		i++;
	}
	return (0);
}