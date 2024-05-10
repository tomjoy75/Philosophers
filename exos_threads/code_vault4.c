/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_vault4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:28 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/10 17:01:48 by tjoyeux          ###   ########.fr       */
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
	for (int i = 0; i < 10000000; i++)
	{
		pthread_mutex_lock(&mutex);
		mails++;
		pthread_mutex_unlock(&mutex);
	}
		// read mails
		// increment in register
		// write mails
}

// void	*routine2()
// {
// 	sleep(2);
// 	printf("Value of x : %d\n", x);
// }

int	main(int argc, char **argv)
{
	pthread_t	t1, t2, t3, t4;
	
	pthread_mutex_init(&mutex, NULL);
	if (pthread_create(&t1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL))
		return (2);
	if (pthread_create(&t3, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&t4, NULL, &routine, NULL))
		return (2);
	if (pthread_join(t1, NULL))
		return (1);
	if (pthread_join(t2, NULL))
		return (2);
	if (pthread_join(t3, NULL))
		return (1);
	if (pthread_join(t4, NULL))
		return (2);
	pthread_mutex_destroy(&mutex);
	printf("Number of mails : %d\n", mails);
	return (0);
}
//https://www.youtube.com/watch?v=oq29KUy29iQ&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=4