/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_routine.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:56:21 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/12 17:21:12 by esoulard         ###   ########.fr       */
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
//	printf("time [%lld]\n", phi->time - phi->start);
	return (EXIT_SUCCESS);
}

int		is_dead(t_phi **phi)
{
	if (get_time(*phi) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	// printf("GETIME OUT phi#%d time [%lld]\n", (*phi)->cur, (*phi)->time);
	if (((*phi)->time - (*phi)->last_meal)
		>= (*phi)->t_die)
	{
		(*phi)->status = DEAD;
		if (action_msg(*phi, "died"))
			return (EXIT_FAILURE);
		return (2);
	}

	return (EXIT_SUCCESS);
}

int		grab_forks(t_phi *tmp)
{
	if (get_time(tmp) ||
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_a]) != 0)
		return (EXIT_FAILURE);
	(*tmp->fork)[tmp->fork_a] = NOT_AVAIL;
	if (action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	if (get_time(tmp) ||
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_b]) != 0)
		return (EXIT_FAILURE);
	(*tmp->fork)[tmp->fork_b] = NOT_AVAIL;
	if (action_msg(tmp, "has taken a fork"))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int		go_eat(t_phi **tmp)
{
	while (((*(*tmp)->fork)[(*tmp)->fork_a] == NOT_AVAIL
		|| (*(*tmp)->fork)[(*tmp)->fork_b] == NOT_AVAIL))
		if (is_dead(tmp))
			break ;
	// printf("GOEAT BEF phi#%d time [%lld]\n", (*tmp)->cur, (*tmp)->time);
	if (!is_dead(tmp))
	{
		// printf("GOEAT AFT phi#%d time [%lld]\n", (*tmp)->cur, (*tmp)->time);
		if (grab_forks(*tmp) || update_last_meal(tmp) ||
			action_msg((*tmp), "is eating") ||
			usleep((*tmp)->t_eat * 1000) < 0)
			return (EXIT_FAILURE);
		(*(*tmp)->fork)[(*tmp)->fork_a] = AVAIL;
		(*(*tmp)->fork)[(*tmp)->fork_b] = AVAIL;
		if (pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_a]) != 0 ||
			pthread_mutex_unlock(&(*(*tmp)->mutex)[(*tmp)->fork_b]) != 0)
			return (EXIT_FAILURE);
	}
	(*tmp)->status++;
	return (EXIT_SUCCESS);
}

void	*handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	// if (update_last_meal(&tmp) && (g_ret = 1) == 1)
	// 	return (NULL);
	// printf("TEST BEF phi#%d time [%lld]\n", tmp->cur, tmp->time);
	//is_dead(&tmp);
	// printf("TEST AFT phi#%d time [%lld]\n", tmp->cur, tmp->time);
	while (is_dead(&tmp) == 0)
	{
		if (tmp->status == EATS && go_eat(&tmp) && (g_ret = 1) == 1)
			return (NULL);
		if (tmp->status == SLEEPS && is_dead(&tmp) == 0)
		{
			if (action_msg(tmp, "is sleeping") == EXIT_FAILURE &&
				(g_ret = 1) == 1)
				return (NULL);
			if (!is_dead(&tmp) && (tmp->tmp = ((tmp->time + tmp->t_sleep) - tmp->last_meal)) >= tmp->t_die)
				tmp->t_sleep = (tmp->last_meal + tmp->t_die) - tmp->time;
			printf("tmp tsleep %ld tmp %d\n", tmp->t_sleep, tmp->tmp);
			if (usleep(tmp->t_sleep * 1000) < 0 && (g_ret = 1) == 1)
				return (NULL);
			tmp->status++;
		}
		if (tmp->status == THINKS && is_dead(&tmp) == 0)
		{
			if (action_msg(tmp, "is thinking") == EXIT_FAILURE &&
				(g_ret = 1) == 1)
				return (NULL);
			while (((*tmp->fork)[tmp->fork_a] == NOT_AVAIL
				|| (*tmp->fork)[tmp->fork_b] == NOT_AVAIL)
				&& !is_dead(&tmp))
			{
				if (usleep(10) < 0)
					return (NULL);
			}
			if (!is_dead(&tmp) && (tmp->time - tmp->last_meal) < (tmp->t_die) &&
				usleep((tmp->t_die - (tmp->time - tmp->last_meal))
					/ (tmp->total) * 100) < 0)
				return (NULL);
			tmp->status = EATS;
		}
	}
	return (NULL);
}
