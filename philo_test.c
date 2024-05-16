/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 10:15:05 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/16 16:45:06 by tjoyeux          ###   ########.fr       */
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
	pthread_mutex_t	*r_fork;
	int				state;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
}				t_philo;			

typedef struct s_rules
{
	int	nb_philo;
	int	time_to_eat;
	int	time_to_sleep;
	int	time_to_die;
}				t_rules;

void	timestamp(t_philo philo)
{
	if (philo.state == 1)
		printf("timestamp_in_ms %d has taken a fork", philo.id);
}

void	*eat(void *phi)
{
	t_philo	*philo = (t_philo *)phi; 
	while (1)
	{
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(&(philo->l_fork));
			pthread_mutex_lock(philo->r_fork);
		}
		else	
		{
			pthread_mutex_lock(philo->r_fork);
			pthread_mutex_lock(&(philo->l_fork));
		}
		philo->state == 1;
		timestamp(*philo);
//		printf("Philo's [%d] eating\n", philo->id);
		usleep(philo->time_to_sleep * 1000);	
		pthread_mutex_unlock(&(philo->l_fork));
		pthread_mutex_unlock(philo->r_fork);
		printf("Philo's [%d] sleeping\n", philo->id);
		usleep(philo->time_to_sleep * 1000);	
		printf("Philo's [%d] thinking\n", philo->id);
	}
}

int	main()
{
	int		nb_philo = 4;
	t_philo	philos[nb_philo];
	int		i = 0;
	// Creer tableau de philo
	// Remplir ces caracteristiques
	while (i < nb_philo)
	{
		philos[i].id = i + 1;
		pthread_mutex_init(&(philos[i].l_fork), NULL);
		if (i == nb_philo - 1)
			philos[i].r_fork = &philos[0].l_fork;
		else
			philos[i].r_fork = &philos[i + 1].l_fork;
		philos[i].time_to_eat = 100;
		philos[i].time_to_sleep = 1000;
		philos[i].time_to_die = 100;
		i++;
	}
	// Envoyer programme
	i = 0;
	while (i < nb_philo)
	{
		if (philos[i].id % 2 == 1 )
			usleep(100);
		pthread_create(&(philos[i].t_id), NULL, &eat, (void *)&philos[i]);
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