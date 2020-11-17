/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/17 17:48:03 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	forecast(t_phi *tmp, long action_time)
{
	if (((tmp->time + action_time) - tmp->last_meal) >= tmp->t_die)
		return (tmp->last_meal + tmp->t_die - tmp->time);
	return (action_time);
}

/*
** LES FOURCHETTES SONT NUMEROTEES EN DECALE A GAUCHE DE CHAQUE PHILO
** EX: Phi 7 à à sa gauche la fork7 et a droite la fork8
** (OU 0 SIL EST LE DERNIER)
*/

int		get_time(t_phi *phi)
{
	if (gettimeofday(&(phi->tv), NULL) < 0)
		return (EXIT_FAILURE);
	phi->time = phi->tv.tv_sec * 1000 + phi->tv.tv_usec / 1000;
	return (EXIT_SUCCESS);
}

int		is_dead(t_phi **phi)
{
	if (*((*phi)->end) == DEAD || (((*phi)->nb_meals) != -1 &&
		((*phi)->ct_meals) >= ((*phi)->nb_meals)))
		return (2);
	if (get_time(*phi) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (((*phi)->time - (*phi)->last_meal)
		>= (*phi)->t_die)
	{
		(*phi)->status = DEAD;
		if (*((*phi)->end) != DEAD && action_msg(*phi, "died"))
			return (EXIT_FAILURE);
		*((*phi)->end) = DEAD;
		return (2);
	}
	return (EXIT_SUCCESS);
}

int		go_think(t_phi **tmp)
{
	if (is_dead(tmp) || action_msg(*tmp, "is thinking"))
		return (EXIT_FAILURE);
	while (((*(*tmp)->fork)[(*tmp)->fork_a] == NOT_AVAIL
		|| (*(*tmp)->fork)[(*tmp)->fork_b] == NOT_AVAIL)
		&& !is_dead(tmp))
		if (usleep(10) < 0)
			return (EXIT_FAILURE);
	if (is_dead(tmp))
		return (EXIT_FAILURE);
	(*tmp)->status = EATS;
	return (EXIT_SUCCESS);
}

void	*handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	while (!is_dead(&tmp))
	{
		if (tmp->status == EATS && !is_dead(&tmp) && go_eat(&tmp))
			return (NULL);
		if (tmp->status == SLEEPS && !is_dead(&tmp))
		{
			if (is_dead(&tmp) || action_msg(tmp, "is sleeping"))
				return (NULL);
			if (is_dead(&tmp) ||
				(usleep(forecast(tmp, tmp->t_sleep) * 1000) < 0))
				return (NULL);
			tmp->status++;
		}
		if (tmp->status == THINKS && !is_dead(&tmp) && go_think(&tmp))
			return (NULL);
	}
	return (NULL);
}
