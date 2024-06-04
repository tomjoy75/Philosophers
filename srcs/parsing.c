/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:58:11 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/05 00:30:08 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline int	is_number(char c)
{
	return (c >= '0' && c <= '9');
}

static inline int	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

// Check is correctly formated number
// return 1 if correct, 0 if not
char	*is_valid_arg(char *str)
{
	int		len;
	char	*num;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		return (error_exit_null("Only positive values\n"));
	if (!is_number(*str))
		return (error_exit_null("Please reformat the number\n"));
	num = str;
	while (is_number(*str))
	{
		str++;
		len++;
	}
	if (len > 10)
		return (error_exit_null("Arg should't be > INT_MAX\n"));
	return (num);
}

int	ft_atol(char *str)
{
	long	n;

	n = 0;
	str = is_valid_arg(str);
	if (!str)
		return (-1);
	while (is_number(*str))
	{
		n *= 10;
		n += (*str - 48);
		str++;
	}
	if (n > INT_MAX)
		return (error_exit("Arg should't be > INT_MAX\n"));
	return (n);
}

// Check and parse the arguments
// Check : numbers, not > INT_MAX, timestamps > 60 ms
// Return value: 0 in case of error
int	parse_input(t_rules *rules, int argc, char **argv)
{
	rules->nb_philo = ft_atol(argv[1]);
	rules->nb_eating = rules->nb_philo;
	rules->time_to_die = ft_atol(argv[2]);
	rules->time_to_eat = ft_atol(argv[3]);
	rules->time_to_sleep = ft_atol(argv[4]);
	rules->error_flag = 0;
	rules->write_off = 0;
	if (argc == 5)
		rules->nb_of_meals = -2;
	else if (argc == 6)
		rules->nb_of_meals = ft_atol(argv[5]);
	if (rules->nb_philo == -1 || rules->time_to_die == -1
		|| rules->time_to_eat == -1 || rules->time_to_sleep == -1
		|| rules->nb_of_meals == -1)
		return (1);
	if (!rules->nb_philo)
		return (error_exit("At least 1 philosopher\n"));
//	rules->finished = 0;
	gettimeofday(&(rules->tv_beg), NULL); //A proteger
//	rules->last_eat_max = 0;
//	if (rules->nb_philo > 1)
//		rules->priority = 2;
//	printf("value of nb_philo in parse_input : %d\n", rules->nb_philo);
	if (pthread_mutex_init(&rules->global_mutex, NULL))
		return (1);
	// if (pthread_mutex_init(&rules->eating_mutex, NULL))
	// 	return (1);
	return (0);
}
