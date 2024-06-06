/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 16:30:47 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/06/06 16:36:41 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_exit(const char *error)
{
	printf(RED"Error :\n\t"ITALIC"%s\n"RESET, error);
	return (1);
}

void	*error_exit_null(const char *error)
{
	printf(RED"Error :\n\t"ITALIC"%s\n"RESET, error);
	return (NULL);
}

void	clean_all(t_philo *philos, int n_mutex)
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

void	mtx_printf_noarg(t_rules *rules, char *str)
{
	pthread_mutex_lock(&rules->global_mutex);
	printf("%s\n", str);
	pthread_mutex_unlock(&rules->global_mutex);
}

void	mtx_printf_arg(t_rules *rules, char *str, int i)
{
	pthread_mutex_lock(&rules->global_mutex);
	printf("%s\t[%i]\n", str, i);
	pthread_mutex_unlock(&rules->global_mutex);
}
