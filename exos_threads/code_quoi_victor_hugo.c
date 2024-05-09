/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_quoi_victor_hugo.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:54:28 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/09 15:11:22 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

# define NC	"\e[0m"
# define YELLOW	"\e[1;33m"

void	*thread_routine(void *data)
{
	pthread_t	tid;

	tid = pthread_self();
	printf("%sThread [%ld]: The heaviest burden is to exist without living.%s\n", YELLOW, tid, NC);
	return (NULL);
}

int	main(void)
{
	pthread_t	tid1;
	pthread_t	tid2;

	pthread_create(&tid1, NULL, thread_routine,NULL);
	printf("Create first thread [%ld]\n", tid1);
	pthread_create(&tid2, NULL, thread_routine,NULL);
	printf("Create second thread [%ld]\n", tid2);
	pthread_join(tid1, NULL);
	printf("Joining first thread [%ld]\n", tid1);
	pthread_join(tid2, NULL);
	printf("Joining second thread [%ld]\n", tid2);
	return (0);
}

