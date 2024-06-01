/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:49:00 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/01 17:01:01 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(int nb_philo, t_philo *philos, t_rules *rules)
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
		philos[i].r_fork = &philos[(i + 1) % nb_philo].l_fork;
		philos[i].r_locked = &philos[(i + 1) % nb_philo].l_locked;
		i++;
	}
	rules->philos = philos;
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	memset(&rules, 0, sizeof(t_rules));
	if (parse_input(&rules, argc, argv))
		return (1);
	printf("value of nb_philo in main after parse_input : %d\n", rules.nb_philo);
	rules.philos = malloc(sizeof(t_philo) * rules.nb_philo);
	if (!rules.philos)
		return (1);
	if (init_philos(rules.nb_philo, rules.philos, &rules))
		return (1);
	if (start_simulation(rules.philos, &rules))
		return (1);
	clean_all(rules.philos, rules.nb_philo);
	return (0);
}
