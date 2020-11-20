/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_food.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 12:07:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/20 18:37:24 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		update_last_meal(t_phi **phi)
{
	if (get_time(*phi) < 0)
		return (EXIT_FAILURE);
	(*phi)->last_meal = (*phi)->time;
	return (EXIT_SUCCESS);
}

int		grab_forks(t_phi *tmp)
{
	if (pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_a]) != 0)
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	if (pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_b]) != 0)
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
			is_dead(tmp) || action_msg((*tmp), "is eating") ||
			nap_time(*tmp, forecast((*tmp), (*tmp)->t_eat)))
			return (EXIT_FAILURE);
		++((*tmp)->ct_meals);
		if (pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_a]) != 0 ||
			pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_b]) != 0)
			return (EXIT_FAILURE);
	}
	(*tmp)->status++;
	return (EXIT_SUCCESS);
}
