/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_gettimeofday.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjoyeux <tjoyeux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 12:38:49 by tjoyeux           #+#    #+#             */
/*   Updated: 2024/05/13 14:04:26 by tjoyeux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main(void)
{
	int	error;
	int	i = 5;
	int passed; 
	struct timeval	tv_beg;
	struct timeval	tv_act;

	error = gettimeofday(&tv_beg, NULL);
	if (error)
		return (1);
	printf ("Time :\n\tsec :%ld\n\tmicrosec :%ld\n", tv_beg.tv_sec, tv_beg.tv_usec);
	while (i)
	{
		sleep(1);
		error = gettimeofday(&tv_act, NULL);
		if (error)
			return (1);
		passed = (((tv_act.tv_sec - tv_beg.tv_sec) * 1000) + (tv_act.tv_usec - tv_beg.tv_usec) / 1000);
		printf ("\nTime passed:\n\t%d,%ld millisec\n", passed, (tv_act.tv_usec - tv_beg.tv_usec) % 1000);		
		i--;
	}
}
	
