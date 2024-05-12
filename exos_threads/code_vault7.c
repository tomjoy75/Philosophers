/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_vault7.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/12 22:08:49 by joyeux            #+#    #+#             */
/*   Updated: 2024/05/12 22:38:59 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int	primes[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

void	*routine(void *arg)
{
//	sleep (1);
	int index = *(int *)arg;
	printf("%d ", primes[index]);
	free (arg);
}

int	main(int argc, char **argv)
{
	pthread_t	th[10];
	int			i;
	int			*a;

	for (i = 0; i < 10; i++)
	{
		a = malloc(sizeof(int));
		*a = i;
		if (pthread_create(&th[i], NULL, &routine, a))
			perror("Failed to create thread");
	}
	for (i = 0; i < 10; i++)
	{
		if (pthread_join(th[i], NULL))
			perror("Failed to join thread");
	}
	return (0);
}