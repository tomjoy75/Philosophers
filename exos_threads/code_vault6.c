/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   code_vault6.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joyeux <joyeux@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 00:50:54 by joyeux            #+#    #+#             */
/*   Updated: 2024/05/11 01:33:18 by joyeux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

void	*roll_dice()
{
	int value = (rand() % 6) + 1;
	int *result = malloc(sizeof(int));
	*result = value;
//	printf("%d\n", value);
	return ((void *) result);
}
int	main(int argc, char **argv)
{
	pthread_t	th[8];
	int			*res;
	int			i;

	srand(time(NULL));
	for (i = 0; i < 8; i++)
	{
		if(pthread_create(&th[i], NULL, &roll_dice, NULL))
			return (1);
	}
	for (i = 0; i < 8; i++)
	{
		if (pthread_join(th[i], (void **) &res))
			return (2);
		printf("[%p]Value %d : %d\n",res, i, *res);
	}
	free(res);
	return (0);
}