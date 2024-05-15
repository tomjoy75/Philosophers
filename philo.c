/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 13:24:40 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/15 18:34:15 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_number(char *str)
{
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

// Check arguments
// Return 0 if args are corrects
int	check_arguments(int argc, char **argv)
{
	int	i;

	if (argc > 6 || argc < 5)
	{
		printf("\e[31mShould be in the form :\n\t\e[0;3m./philo "
			"number_of_philosophers time_to_die time_to_eat time_to_sleep "
			"[number_of_times_each_philosopher_must_eat]\n\e[0m");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
		{
			printf("\e[31mError :\n\t\e[0;3m\"%s\"\e[0m is not a valid "
				"strictly positive number\n\e[0m", argv[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*routine()
{
	printf("Philosopher #[%ld] created\n", pthread_self());	
}

int	main(int argc, char **argv)
{
	int		nb_philo;
	t_philo	*philos;
	int		i;

	if (check_arguments(argc, argv))
		return (1);
	nb_philo = ft_atoi(argv[1]);
	philos = malloc(sizeof(t_philo) * (nb_philo + 1));
	if (!philos)
		return (1);
/*	i = 0;
	while (i < nb_philo)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
		{
			while (--i >= 0)
				free(philos[i]);
			return (free (philos), 1);
		}
		i++;
	}*/
	i = 0;
	while (i < nb_philo)
	{
		philos[i].num = i + 1;
		pthread_create(&philos[i].tid, NULL, &routine, NULL);
		i++;
	}
	philos[i].num = -1;
	i = 0;
	while (i < nb_philo)
	{
		pthread_join(philos[i].tid, NULL);
		printf("Philosopher num %d [%ld] joined\n", philos[i].num, philos[i].tid);
		i++;
	}
	return (0);
}
