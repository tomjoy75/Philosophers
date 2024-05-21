/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_new.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:49:00 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/21 15:07:30 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_rules	rules;
// error check and fill table
// 1) parse input
	if (parse_input(&rules, argc, argv))
		return (1);
	printf("nb_philo\t%d\ntime to eat\t%d\ntime to sleep\t%d\ntime to die\t%d\n", rules.nb_philo, rules.time_to_eat, rules.time_to_sleep, rules.time_to_die); 


// init the structs
// 2) init
// 3) start_simulation
// __end when all philos are fed | 1 philo died
// 4) clean_tables //check leaks
	return (0);
}
