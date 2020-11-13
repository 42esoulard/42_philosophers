/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/13 21:13:58 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <stdio.h>

int		launch_threads(t_phi *phi, pthread_t *thread_tab)
{
	int i;

	i = 0;
	while (i < phi[0].total)
	{
		thread_tab[i] = 0;
		phi[i].start = phi[0].time;
		phi[i].last_meal = phi[0].time;
		//printf(">>> time %lld start  %lld last meal  %lld\n", phi[i].time, phi[i].start, phi[i].last_meal);
		if (pthread_create(&thread_tab[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
		// if (usleep(50) < 0)
		// 	return (EXIT_FAILURE);
		i++;
		// i += 2;
		// if (i >= phi[0].total && i % 2 == 0)
		// 	i = 1;
	}
	
	//getchar();
	return (EXIT_SUCCESS);
}

int		main(int ac, char **av)
{
	int				*fork;
	pthread_mutex_t *mutex;
	t_phi			*phi;
	pthread_t		*thread_tab;
	int				i;
	// int j;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE ||
		!(fork = malloc(sizeof(int) * phi[0].fork_total)) ||
		!(mutex = malloc(sizeof(pthread_mutex_t) * phi[0].fork_total))
		|| !(thread_tab = malloc(sizeof(pthread_t) * phi[0].total)) ||
		!(phi[0].end = (int *)malloc(sizeof(int *))))
		return (EXIT_FAILURE);
	i = -1;
	*(phi[0].end) = 0;
	while (++i < phi[0].fork_total)
	{
		fork[i] = AVAIL;
		if (pthread_mutex_init(&mutex[i], NULL) != 0)
			return (EXIT_FAILURE);
		phi[i].fork = &fork;
		phi[i].mutex = &mutex;
		phi[i].end = phi[0].end;
	}
	if (get_time(&phi[0]) || launch_threads(phi, thread_tab))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < phi[0].total)
	{
		//printf(">>>>>>>>>>>>HERE %d\n", i);
		if (pthread_join(thread_tab[i], NULL))
			return (EXIT_FAILURE);
		// j = -1;
		// while (++j < phi[0].total)
		// 	if (phi[j].status == DEAD)
		// 		return (free_all(phi, thread_tab));
		//printf(">>>>>>>>>>>>2HERE\n");
	}
	return (free_all(phi, thread_tab));
}

/*
**			Fin du programme : Si un philosophe meurt, ou si tous les
**			philosophes ont mangé number_of_time_each_philosophers_must_eat fois
**			(s'il est défini, sinon l'init à -1).
**			Any of the threads in the process calls exit(3), or the main thread
**			performs a return from main(). This causes the termination of all
**			threads in the process.
**			Il me faut comme fonctions :
**					handle_philo
*/
