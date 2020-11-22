/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/22 17:56:53 by esoulard         ###   ########.fr       */
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

long long	get_time(t_phi *phi)
{
	if (gettimeofday(&(phi->tv), NULL) < 0)
		return (EXIT_FAILURE);
	phi->time = phi->tv.tv_sec * 1000 + phi->tv.tv_usec / 1000;
	return (phi->time);
}

int			is_dead(t_phi **phi)
{
	if (((*phi)->nb_meals) != -1 &&
		((*phi)->ct_meals) >= ((*phi)->nb_meals))
		exit(EXIT_SUCCESS);
	if (get_time(*phi) < 0)
		exit(EXIT_FAILURE);
	if (((*phi)->time - (*phi)->last_meal)
		>= (*phi)->t_die)
	{
		(*phi)->status = DEAD;
		if (action_msg(*phi, "died"))
			exit(EXIT_FAILURE);
		exit(3);
	}
	return (EXIT_SUCCESS);
}

int			handle_phi(void *phi)
{
	t_phi		*tmp;
	pthread_t	chk;

	tmp = (t_phi *)phi;
	if (pthread_create(&chk, NULL, (void*)chk_death, tmp))
		exit(EXIT_FAILURE);
	while (!is_dead(&tmp))
	{
		if (tmp->status == EATS)
		{
			if (go_eat(&tmp))
				exit(EXIT_FAILURE);
		}
		else
		{
			if (action_msg(tmp, "is sleeping") ||
				nap_time(phi, forecast(tmp, tmp->t_sleep))
				|| action_msg(tmp, "is thinking"))
				exit(EXIT_FAILURE);
			tmp->status = EATS;
		}
	}
	exit(EXIT_SUCCESS);
}
