/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/23 16:12:32 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int			nap_time(t_phi *phi, int duration)
{
	time_t timestamp;

	timestamp = get_time(phi);
	while (get_time(phi) - timestamp < duration)
		if (usleep(200) < 0)
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

long		forecast(t_phi *tmp, long action_time)
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
	return (phi->time);
}

int			is_dead(t_phi **phi)
{
	if (pthread_mutex_lock(((*phi)->eat_mx)) != 0)
		return (EXIT_FAILURE);
	if (*((*phi)->end) == DEAD || (((*phi)->nb_meals) != -1 &&
		((*phi)->ct_meals) >= ((*phi)->nb_meals)))
	{
		pthread_mutex_unlock(((*phi)->eat_mx));
		return (2);
	}
	if (get_time(*phi) < 0)
		return (EXIT_FAILURE);
	if (((*phi)->time - (*phi)->last_meal)
		>= (*phi)->t_die)
	{
		(*phi)->status = DEAD;
		if (*((*phi)->end) != DEAD && action_msg(*phi, "died"))
			return (EXIT_FAILURE);
		*((*phi)->end) = DEAD;
		pthread_mutex_unlock(((*phi)->eat_mx));
		return (2);
	}
	if (pthread_mutex_unlock(((*phi)->eat_mx)) != 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

void		*handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	while (!is_dead(&tmp))
	{
		if (tmp->status == EATS)
		{
			if (go_eat(&tmp))
				return (NULL);
		}
		else
		{
			if (action_msg(tmp, "is sleeping") ||
				nap_time(phi, forecast(tmp, tmp->t_sleep))
				|| action_msg(tmp, "is thinking"))
				return (NULL);
			tmp->status = EATS;
		}
	}
	return (NULL);
}