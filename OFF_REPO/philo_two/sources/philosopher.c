/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/23 18:06:44 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		chk_death(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	while (tmp->status != DEAD)
	{
		usleep(1000);
		if (is_dead(&tmp))
			break ;
	}
	return (0);
}

int		launch_threads(t_phi *phi, pthread_t *c_thr, pthread_t *p_thr)
{
	int	i;

	i = -1;
	while (++i < phi[0].total)
	{
		phi[i].start = phi[0].time;
		phi[i].last_meal = phi[0].time;
		if (pthread_create(&p_thr[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
		if (pthread_create(&c_thr[i], NULL, (void *)chk_death, &phi[i]))
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		join_threads(t_phi *phi, pthread_t *chk_thr, pthread_t *phi_thr, sem_t *eat_sem)
{
	int i;

	i = -1;
	while (++i < phi[0].total)
		if (pthread_join(chk_thr[i], NULL))
			return (EXIT_FAILURE);
	i = -1;
	while (++i < phi[0].total)
		if (pthread_join(phi_thr[i], NULL))
			return (EXIT_FAILURE);
	return (free_all(phi, chk_thr, phi_thr, eat_sem));
}

int		main(int ac, char **av)
{
	t_phi			*phi;
	pthread_t		*chk_thr;
	pthread_t		*phi_thr;
	sem_t			*forks_sem;
	sem_t			*eat_sem;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE ||
		!(chk_thr = malloc(sizeof(pthread_t) * phi[0].total))
		|| !(phi_thr = malloc(sizeof(pthread_t) * phi[0].total))
		|| !(phi[0].end = (int *)malloc(sizeof(int *)))
		|| !(eat_sem = malloc(sizeof(sem_t) * phi[0].total))
		|| init_tabs(&phi, &forks_sem, &eat_sem))
		return (EXIT_FAILURE);
	if (get_time(&phi[0]) < 0 || launch_threads(phi, chk_thr, phi_thr))
		return (EXIT_FAILURE);
	return (join_threads(phi, chk_thr, phi_thr, eat_sem));
}

/*
**			Fin du programme : Si un philosophe meurt, ou si tous les
**			philosophes ont mangé number_of_time_each_philosophers_must_eat fois
**			(s'il est défini, sinon l'init à -1).
*/