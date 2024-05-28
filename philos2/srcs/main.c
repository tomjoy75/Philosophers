/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:49:00 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/28 11:03:19 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philos(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philos[i].id = i + 1;
		if (gettimeofday(&(rules->philos[i].last_meal_time), NULL))
			return (clean_all(rules->philos, rules->nb_philo), 1);
		rules->philos[i].meals_eaten = 0;
		if (pthread_mutex_init(&(rules->philos[i].mutex), NULL))
			return (clean_all(rules->philos, i), 1);
		if (pthread_mutex_init(&(rules->philos[i].l_fork), NULL))
			return (clean_all(rules->philos, i), 1);
		if (i == rules->nb_philo - 1)
			rules->philos[i].r_fork = &rules->philos[0].l_fork;
		else
			rules->philos[i].r_fork = &rules->philos[i + 1].l_fork;
		rules->philos[i].rules = rules;
		i++;
	}
	if (pthread_mutex_init(&rules->general_mutex, NULL))
		return (clean_all(rules->philos, i), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_rules	rules;

	if (parse_input(&rules, argc, argv))
		return (1);
	if (init_philos(&rules))
		return (1);
// init the structs
// 2) init
// 3) start_simulation
// __end when all philos are fed | 1 philo died
	/*if (start_simulation(philos, &rules))
		return (1);*/
// 4) clean_tables //check leaks
	clean_all(rules.philos, rules.nb_philo);
	return (0);
}
