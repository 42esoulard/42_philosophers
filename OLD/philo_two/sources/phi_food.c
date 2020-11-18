/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_food.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 12:07:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/16 19:15:22 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		update_last_meal(t_phi **phi)
{
	if (get_time(*phi) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	(*phi)->last_meal = (*phi)->time;
	return (EXIT_SUCCESS);
}

int		grab_forks(t_phi *tmp)
{
	if (is_dead(&tmp) || (get_time(tmp)) ||
		sem_wait(*(tmp->forks_sem)))
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || (get_time(tmp)) ||
		sem_wait(*(tmp->forks_sem)))
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		go_eat(t_phi **tmp)
{
	if (!is_dead(tmp))
	{
		if (grab_forks(*tmp) || update_last_meal(tmp) ||
			action_msg((*tmp), "is eating") ||
			usleep(forecast((*tmp), (*tmp)->t_eat) * 1000) < 0)
			return (EXIT_FAILURE);
		++((*tmp)->ct_meals);
		if (sem_post((*(*tmp)->forks_sem)) || sem_post((*(*tmp)->forks_sem)))
			return (EXIT_FAILURE);
	}
	(*tmp)->status++;
	return (EXIT_SUCCESS);
}
