/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:49:00 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/22 16:26:04 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// Clean all the mutexes and free philos
/*void	clean_all(t_philo *philos, int n_mutex)
{
	int	i;

	i = 0;
	while (i < n_mutex)
	{
		pthread_mutex_destroy(&(philos[i].l_fork));
		i++;
	}
	free (philos);
}
*/
int	init_philos(int nb_philo, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < nb_philo)
	{
		philos[i].id = i + 1;
		if (pthread_mutex_init(&(philos[i].l_fork), NULL))
			return (clean_all(philos, i), 1);
		philos[i].l_locked = 0;
		if (gettimeofday(&(philos[i].last_eat), NULL))
			return (clean_all(philos, nb_philo), 1);
		philos[i].meal = 0;
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
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules	rules;
	t_philo	*philos;
// error check and fill table
// 1) parse input
	if (parse_input(&rules, argc, argv))
		return (1);
//	printf("nb_philo\t%d\ntime to eat\t%d\ntime to sleep\t%d\ntime to die\t%d\n", rules.nb_philo, rules.time_to_eat, rules.time_to_sleep, rules.time_to_die); 
	philos = malloc(sizeof(t_philo) * rules.nb_philo);
	if (!philos)
		return (1);
	if (init_philos(rules.nb_philo, philos))
		return (1);
// init the structs
// 2) init
// 3) start_simulation
// __end when all philos are fed | 1 philo died
	if (start_simulation(philos, &rules))
		return (1);
// 4) clean_tables //check leaks
	clean_all(philos, rules.nb_philo);
	return (0);
}
