/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 09:58:11 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/23 00:49:35 by tjoyeux          ###   ########.fr       */
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
		return (error_exit_NULL("Only positive values\n"));
	if (!is_number(*str))
		return (error_exit_NULL("Please reformat the number\n"));
	num = str;
	while (is_number(*str))
	{
		str++;
		len++;
	}
	if (len > 10)
		return (error_exit_NULL("Arg should't be > INT_MAX\n"));
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
	if (argc < 5 || argc > 6)
		return (error_exit("Should be in the form :\n\t\e[0;3m./philo "
				"number_of_philosophers time_to_die time_to_eat time_to_sleep "
				"[number_of_times_each_philosopher_must_eat]\n"));
	rules->nb_philo = ft_atol(argv[1]);
	rules->nb_eating = rules->nb_philo;
	rules->time_to_die = ft_atol(argv[2]);
	rules->time_to_eat = ft_atol(argv[3]);
	rules->time_to_sleep = ft_atol(argv[4]);
	if (argc == 5)
		rules->nb_of_meals = -2;
	else if (argc == 6)
		rules->nb_of_meals = ft_atol(argv[5]);
	if (rules->nb_philo == -1 || rules->time_to_die == -1
		|| rules->time_to_eat == -1 || rules->time_to_sleep == -1
		|| rules->nb_of_meals == -1)
		return (-1);
	if (!rules->nb_philo)
		return (error_exit("At least 1 philosopher\n"));
//	rules->finished = 0;
	gettimeofday(&(rules->tv_beg), NULL);
	return (0);
}
