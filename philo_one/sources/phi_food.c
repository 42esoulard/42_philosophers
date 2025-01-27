/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_food.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 12:07:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/27 11:30:20 by esoulard         ###   ########.fr       */
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
	if (pthread_mutex_lock(&(*tmp->fk_mx)[tmp->fork_a]) != 0)
		return (EXIT_FAILURE);
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
	{
		pthread_mutex_unlock(&(*tmp->fk_mx)[tmp->fork_a]);
		return (EXIT_FAILURE);
	}
	if (pthread_mutex_lock(&(*tmp->fk_mx)[tmp->fork_b]) != 0)
	{
		pthread_mutex_unlock(&(*tmp->fk_mx)[tmp->fork_a]);
		return (EXIT_FAILURE);
	}
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
	{
		pthread_mutex_unlock(&(*tmp->fk_mx)[tmp->fork_a]);
		pthread_mutex_unlock(&(*tmp->fk_mx)[tmp->fork_b]);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int		go_eat(t_phi **tmp)
{
	int ret;

	ret = EXIT_SUCCESS;
	if (!is_dead(tmp))
	{
		if (grab_forks(*tmp))
			return (EXIT_FAILURE);
		if (update_last_meal(tmp) ||
			pthread_mutex_lock(((*tmp)->eat_mx)) != 0 ||
			action_msg((*tmp), "is eating") ||
			nap_time(*tmp, forecast((*tmp), (*tmp)->t_eat)))
			ret = EXIT_FAILURE;
		++((*tmp)->ct_meals);
		if (pthread_mutex_unlock(&(*(*tmp)->fk_mx)[(*tmp)->fork_a]) != 0 ||
			pthread_mutex_unlock(&(*(*tmp)->fk_mx)[(*tmp)->fork_b]) != 0 ||
			pthread_mutex_unlock((*tmp)->eat_mx) != 0)
			return (EXIT_FAILURE);
		if (usleep(50) < 0)
			return (EXIT_FAILURE);
	}
	(*tmp)->status++;
	return (ret);
}
