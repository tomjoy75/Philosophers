/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 11:52:29 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/22 15:47:54 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

# define BLACK		"\x1b[1;30m"
# define RED		"\x1b[1;31m"
# define GREEN		"\x1b[1;32m"
# define YELLOW		"\x1b[1;33m"
# define BLUE		"\x1b[1;34m"
# define MAGENTA	"\x1b[1;35m"
# define CYAN		"\x1b[1;36m"
# define WHITE		"\x1b[1;37m"
# define ITALIC		"\x1b[34m\x1b[3m"
# define RESET		"\x1b[0m"

typedef struct s_philo
{
	int				id;
	pthread_t		t_id;
	pthread_mutex_t	l_fork;
	int				l_locked;
	pthread_mutex_t	*r_fork;
	int				*r_locked;
	int				state;
	int				meal;
	struct timeval	last_eat;
}				t_philo;

typedef struct s_rules
{
	int				nb_philo;
	int				time_to_eat;
	int				time_to_sleep;
	int				time_to_die;
	struct timeval			tv_beg;
	int				finished;
	int				nb_of_meals;
}				t_rules;

typedef struct s_args
{
	t_philo	*philo;
	t_rules	*rules;
}				t_args;

int	ft_atoi(const char *nptr);
int	error_exit(const char *error);
void	*error_exit_NULL(const char *error);
int	parse_input(t_rules *rules, int argc, char **argv);
void	clean_all(t_philo *philos, int n_mutex);
int	timestamp(t_philo philo, t_rules rules, int state);
int	start_simulation(t_philo *philos, t_rules *rules);

#endif
