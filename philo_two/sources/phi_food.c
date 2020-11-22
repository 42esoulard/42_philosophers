/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_food.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 12:07:53 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/22 14:51:45 by esoulard         ###   ########.fr       */
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

int		double_post(sem_t sema, sem_t semb)
{
	sem_post(sema);
	sem_post(semb);
	return (EXIT_FAILURE);
}

int		grab_forks(t_phi *tmp)
{
	while ((*tmp->forks_ct) < 2)
		if (is_dead(&tmp))
			return (EXIT_FAILURE);
	if (sem_wait(*(tmp->forks_ct_sem)))
		return (EXIT_FAILURE);
	if (sem_wait(*(tmp->forks_sem)))
	{
		sem_post(*(tmp->forks_ct_sem));
		return (EXIT_FAILURE);
	}
	(*tmp->forks_ct)--;
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork")
		|| sem_wait(*(tmp->forks_sem)))
		return (double_post(*(tmp->forks_sem), *(tmp->forks_ct_sem)));
	(*tmp->forks_ct)--;
	sem_post(*(tmp->forks_ct_sem));
	if (is_dead(&tmp) || action_msg(tmp, "has taken a fork"))
		return (double_post(*(tmp->forks_sem), *(tmp->forks_sem)));
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
			is_dead(tmp) || action_msg((*tmp), "is eating") ||
			nap_time(*tmp, forecast((*tmp), (*tmp)->t_eat)))
			ret = EXIT_FAILURE;
		++((*tmp)->ct_meals);
		if (sem_post((*(*tmp)->forks_sem)) || sem_post((*(*tmp)->forks_sem)))
			return (EXIT_FAILURE);
		*((*tmp)->forks_ct) += 2;
	}
	(*tmp)->status++;
	return (ret);
}
