/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_vault5.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:28 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/10 17:33:13 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	mails = 0;
pthread_mutex_t	mutex;

void	*routine()
{
	for (int i = 0; i < 1000000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
}

int	main(int argc, char **argv)
{
	pthread_t	th[4];
	int			i = 0;
	
	pthread_mutex_init(&mutex, NULL);
	for (i = 0;i < 4; i++)
	{
		if (pthread_create(th + i, NULL, &routine, NULL))
		{
			perror("Failed to create thread");
			return (1);
		}
		printf("Thread %d started\n", i);
		if (pthread_join(th[i], NULL))
			return (2);
		printf("Thread %d finished\n", i);
		
	}
	pthread_mutex_destroy(&mutex);
	printf("Number of mails : %d\n", mails);
	return (0);
}
//https://www.youtube.com/watch?v=oq29KUy29iQ&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=4