/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/03 15:20:52 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/27 18:16:25 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int			ft_init_err(char *stra, char *strb, t_phi **phi)
{
	if (stra && ft_putstr(2, stra) < 0)
		return (EXIT_FAILURE);
	if (strb)
	{
		if (ft_putstr(2, ": invalid parameter.\n\n") < 0 ||
			ft_putstr(2, strb) < 0 || ft_putstr(2,
				" should be a positive integer > 0.\n") < 0)
			return (EXIT_FAILURE);
	}
	if (ft_putstr(2, "\nTry: ./philosopher <number_of_philosopher> ") < 0
		|| ft_putstr(2, "<time_to_die> <time_to_eat> <time_to_sleep> ") < 0
		|| ft_putstr(2, "[number_of_time_each_philosophers_must_eat]\n") < 0)
		return (EXIT_FAILURE);
	if (phi)
		return (free_phi(*phi));
	return (EXIT_FAILURE);
}

static void	fill_phi(t_phi **phi, int cur, int total)
{
	(*phi)[cur].cur = cur + 1;
	(*phi)[cur].status = EATS;
	(*phi)[cur].fork_a = cur;
	(*phi)[cur].fork_b = cur + 1;
	if (cur % 2 != 0)
		(*phi)[cur].status = SLEEPS;
	(*phi)[cur].total = total;
	(*phi)[cur].ct_meals = 0;
	if (cur != 0)
	{
		(*phi)[cur].t_eat = (*phi)[0].t_eat;
		(*phi)[cur].t_die = (*phi)[0].t_die;
		(*phi)[cur].t_sleep = (*phi)[0].t_sleep;
		(*phi)[cur].nb_meals = (*phi)[0].nb_meals;
	}
	if (cur + 1 == total && (*phi)[cur].status == EATS)
		(*phi)[cur].status = THINKS;
	if (cur + 1 == total)
	{
		(*phi)[cur].fork_a = cur;
		(*phi)[cur].fork_b = 0;
	}
}

int			init_phi(int ac, char **av, t_phi **phi)
{
	int total;

	total = 0;
	if (ac != 5 && ac != 6)
		return (ft_init_err("Wrong number of parameters!\n", NULL, NULL));
	if ((total = ft_atoi(av[1])) <= 0)
		return (ft_init_err(av[1], PARAM_NB_PHI, NULL));
	if (!(*phi = (t_phi *)malloc(sizeof(t_phi) * total)))
		return (EXIT_FAILURE);
	if ((((*phi)[0]).t_die = ft_atoi(av[2])) <= 0)
		return (ft_init_err(av[2], PARAM_TDIE, phi));
	if (((*phi)[0].t_eat = ft_atoi(av[3])) <= 0)
		return (ft_init_err(av[3], PARAM_TEAT, phi));
	if (((*phi)[0].t_sleep = ft_atoi(av[4])) <= 0)
		return (ft_init_err(av[4], PARAM_TSLEEP, phi));
	if (((*phi)[0].tmp = -1) == -1 && ac == 6)
	{
		if (((*phi)[0].nb_meals = ft_atoi(av[5])) < 0)
			return (ft_init_err(av[5], PARAM_NB_MEAL, phi));
	}
	else
		(*phi)[0].nb_meals = -1;
	while (++((*phi)[0].tmp) < total)
		fill_phi(phi, (*phi)[0].tmp, total);
	return (EXIT_SUCCESS);
}

int			init_tabs(t_phi **phi, pthread_mutex_t **fk_mx,
	pthread_mutex_t **eat_mx)
{
	int				i;
	pthread_mutex_t	*wr_mx;

	*((*phi)[0].end) = 0;
	if (!(wr_mx = malloc(sizeof(pthread_mutex_t)))
		|| pthread_mutex_init(wr_mx, NULL) != 0)
		return (EXIT_FAILURE);
	i = -1;
	while (++i < (*phi)[0].total)
	{
		if (pthread_mutex_init(&((*fk_mx)[i]), NULL) != 0)
			return (EXIT_FAILURE);
		if (pthread_mutex_init(&((*eat_mx)[i]), NULL) != 0)
			return (EXIT_FAILURE);
		(*phi)[i].fk_mx = fk_mx;
		(*phi)[i].end = (*phi)[0].end;
		(*phi)[i].wr_mx = wr_mx;
		(*phi)[i].eat_mx = &((*eat_mx)[i]);
	}
	return (EXIT_SUCCESS);
}
