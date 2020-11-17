/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/17 17:43:52 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

long	forecast(t_phi *tmp, long action_time)
{
	if (((tmp->time + action_time) - tmp->last_meal) >= tmp->t_die)
		return (tmp->last_meal + tmp->t_die - tmp->time);
	return (action_time);
}

int		get_time(t_phi *phi)
{
	if (gettimeofday(&(phi->tv), NULL) < 0)
		return (EXIT_FAILURE);
	phi->time = phi->tv.tv_sec * 1000 + phi->tv.tv_usec / 1000;
	return (EXIT_SUCCESS);
}

int		is_dead(t_phi **phi)
{
	if (((*phi)->nb_meals) != -1 &&
		((*phi)->ct_meals) >= ((*phi)->nb_meals))
		exit(EXIT_SUCCESS);
	if (get_time(*phi) == EXIT_FAILURE)
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

int		go_think(t_phi **tmp)
{
	if (is_dead(tmp) || action_msg(*tmp, "is thinking"))
		return (EXIT_FAILURE);
	if (is_dead(tmp) || (((*tmp)->time - (*tmp)->last_meal) < ((*tmp)->t_die) &&
		usleep(((*tmp)->t_die - ((*tmp)->time - (*tmp)->last_meal))
		/ ((*tmp)->total) * 100) < 0))
		return (EXIT_FAILURE);
	(*tmp)->status = EATS;
	return (EXIT_SUCCESS);
}

int		handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	while (!is_dead(&tmp))
	{
		if (tmp->status == EATS && !is_dead(&tmp) && go_eat(&tmp))
			exit(EXIT_FAILURE);
		if (tmp->status == SLEEPS && !is_dead(&tmp))
		{
			if (is_dead(&tmp) || action_msg(tmp, "is sleeping"))
				exit(EXIT_FAILURE);
			if (is_dead(&tmp) ||
				(usleep(forecast(tmp, tmp->t_sleep) * 1000) < 0))
				exit(EXIT_FAILURE);
			tmp->status++;
		}
		if (tmp->status == THINKS && !is_dead(&tmp) && go_think(&tmp))
			exit(EXIT_FAILURE);
	}
	exit(EXIT_FAILURE);
}
