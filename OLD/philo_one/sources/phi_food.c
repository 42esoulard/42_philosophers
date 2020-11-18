/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_food.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 12:07:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/14 19:21:21 by esoulard         ###   ########.fr       */
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
	if (is_dead(&tmp) || (get_time(tmp) ||
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_a]) != 0))
		return (EXIT_FAILURE);
	(*tmp->fork)[tmp->fork_a] = NOT_AVAIL;
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || (get_time(tmp) ||
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_b]) != 0))
		return (EXIT_FAILURE);
	(*tmp->fork)[tmp->fork_b] = NOT_AVAIL;
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		go_eat(t_phi **tmp)
{
	while (((*(*tmp)->fork)[(*tmp)->fork_a] == NOT_AVAIL
		|| (*(*tmp)->fork)[(*tmp)->fork_b] == NOT_AVAIL))
		if (is_dead(tmp))
			break ;
	if (!is_dead(tmp))
	{
		if (grab_forks(*tmp) || update_last_meal(tmp) ||
			action_msg((*tmp), "is eating") ||
			usleep(forecast((*tmp), (*tmp)->t_eat) * 1000) < 0)
			return (EXIT_FAILURE);
		++((*tmp)->ct_meals);
		(*(*tmp)->fork)[(*tmp)->fork_a] = AVAIL;
		(*(*tmp)->fork)[(*tmp)->fork_b] = AVAIL;
		if (pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_a]) != 0 ||
			pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_b]) != 0)
			return (EXIT_FAILURE);
	}
	(*tmp)->status++;
	return (EXIT_SUCCESS);
}
