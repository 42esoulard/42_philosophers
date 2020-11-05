/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 15:20:52 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/05 17:34:25 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static void	fill_phi(t_phi **phi, int cur, int total)
{
	(*phi)[cur].cur = cur;
	(*phi)[cur].status = EATS;
	if (cur % 2 != 0)
		(*phi)[cur].status = SLEEPS;
	(*phi)[cur].fork_a = cur;
	(*phi)[cur].fork_b = cur + 1;
	(*phi)[cur].total = total;
	if (cur != 0)
	{
		(*phi)[cur].t_eat = (*phi)[0].t_eat;
		(*phi)[cur].t_die = (*phi)[0].t_die;
		(*phi)[cur].t_sleep = (*phi)[0].t_sleep;
		(*phi)[cur].nb_meals = (*phi)[0].nb_meals;
	}
	if (cur + 1 == total && total > 1 && (*phi)[cur].status == EATS)
		(*phi)[cur].status = THINKS;
	if (cur + 1 == total && total > 1)
		(*phi)[cur].fork_b = 0;
}

int			init_phi(int ac, char **av, t_phi **phi)
{
	int total;
	int i;

	total = 0;
	if (ac != 5 && ac != 6)
		return (ft_init_err("Wrong number of parameters!\n", NULL, NULL));
	if ((total = ft_atoi(av[1])) == -1 || total == 0)
		return (ft_init_err(av[1], PARAM_NB_PHI, NULL));
	if (!(*phi = (t_phi *)malloc(sizeof(t_phi) * total)))
		return (EXIT_FAILURE);
	if ((((*phi)[0]).t_die = ft_atoi(av[2])) == -1)
		return (ft_init_err(av[2], PARAM_TDIE, phi));
	if (((*phi)[0].t_eat = ft_atoi(av[3])) == -1)
		return (ft_init_err(av[3], PARAM_TEAT, phi));
	if (((*phi)[0].t_sleep = ft_atoi(av[4])) == -1)
		return (ft_init_err(av[4], PARAM_TSLEEP, phi));
	if ((i = -1) == -1 && ac == 6)
	{
		if (((*phi)[0].nb_meals = ft_atoi(av[5])) == -1)
			return (ft_init_err(av[5], PARAM_NB_MEAL, phi));
	}
	else
		(*phi)[0].nb_meals = -1;
	while (++i < total)
		fill_phi(phi, i, total);
	return (EXIT_SUCCESS);
}
