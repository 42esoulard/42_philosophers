/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/16 19:15:35 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		launch_threads(t_phi *phi, pthread_t *thread_tab)
{
	int i;

	i = -1;
	while (++i < phi[0].total)
	{
		thread_tab[i] = 0;
		phi[i].start = phi[0].time;
		phi[i].last_meal = phi[0].time;
		if (pthread_create(&thread_tab[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		main(int ac, char **av)
{
	t_phi			*phi;
	pthread_t		*thread_tab;
	int				i;
	sem_t			*forks_sem;
	sem_t			*wr_sem;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE ||
		!(thread_tab = malloc(sizeof(pthread_t) * phi[0].total)) ||
		!(phi[0].end = (int *)malloc(sizeof(int *))) ||
		init_tabs(&phi, &forks_sem, &wr_sem))
		return (EXIT_FAILURE);
	if (get_time(&phi[0]) || launch_threads(phi, thread_tab))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < phi[0].total)
		if (pthread_join(thread_tab[i], NULL))
			return (EXIT_FAILURE);
	return (free_all(phi, thread_tab));
}

/*
**			Fin du programme : Si un philosophe meurt, ou si tous les
**			philosophes ont mangé number_of_time_each_philosophers_must_eat fois
**			(s'il est défini, sinon l'init à -1).
*/
