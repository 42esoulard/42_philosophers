/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/20 18:16:50 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <stdio.h>

int		nap_time(t_phi *phi, int duration)
{
	time_t timestamp;

	timestamp = get_time(phi);
	while (get_time(phi) - timestamp < duration)
		if (usleep(200) < 0)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

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


long long	get_time(t_phi *phi)
{
	if (gettimeofday(&(phi->tv), NULL) < 0)
		return (-1);
	phi->time = phi->tv.tv_sec * 1000 + phi->tv.tv_usec / 1000;
	//printf("%lld\n", phi->time);
	return (phi->time);
}

int		is_dead(t_phi **phi)
{
	if (*((*phi)->end) == DEAD || (((*phi)->nb_meals) != -1 &&
		((*phi)->ct_meals) >= ((*phi)->nb_meals)))
		return (2);
	if (get_time(*phi) < 0)
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
	if (action_msg(*tmp, "is thinking"))
		return (EXIT_FAILURE);
	(*tmp)->status = EATS;
	return (EXIT_SUCCESS);
}

void	*handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	//printf("balbl\n");
	while (!is_dead(&tmp))
	{
		
		if (tmp->status == EATS)
		{
			if (go_eat(&tmp))
				return (NULL);
		}
		else
		{
			action_msg(tmp, "is sleeping");
			nap_time(phi, forecast(tmp, tmp->t_sleep));
			action_msg(tmp, "is thinking");
			tmp->status = EATS;
		}
	}
	return (NULL);
}
