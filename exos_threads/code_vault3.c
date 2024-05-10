/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_vault3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:26:28 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/10 15:32:28 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	mails = 0;

void	*routine()
{
	for (int i = 0; i < 1000000; i++)
		mails++;
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
	pthread_t	t1, t2;

	if (pthread_create(&t1, NULL, &routine, NULL))
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL))
		return (2);
	if (pthread_join(t1, NULL))
		return (1);
	if (pthread_join(t2, NULL))
		return (2);
	printf("Number of mails : %d\n", mails);
	return (0);
}
// https://www.youtube.com/watch?v=IKG1P4rgm54&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=2https://www.youtube.com/watch?v=IKG1P4rgm54&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2&index=2
