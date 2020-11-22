/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/22 20:14:13 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int			chk_death(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	while (tmp->status != DEAD)
	{
		usleep(200);
		if (is_dead(&tmp))
			break ;
	}
	exit(3);
}

static void	handle_parent(t_phi *phi, int i, int **pid_tab, int pid)
{
	int status;

	(*pid_tab)[i] = pid;
	if (i + 1 == phi[0].total)
	{
		while (1)
		{
			if (waitpid(-1, &status, 0) < 0 || ((WIFEXITED(status)
				|| WIFSIGNALED(status)) && status != 0))
			{
				while (++i < phi[0].total)
					kill((*pid_tab)[i], SIGINT);
				break ;
			}
		}
	}
}

int			launch_proc(t_phi *phi, int *pid_tab)
{
	int i;
	int pid;

	i = -1;
	while (++i < phi[0].total)
	{
		phi[i].start = phi[0].time;
		phi[i].last_meal = phi[0].time;
		if ((pid = fork()) < 0)
			return (EXIT_FAILURE);
		else if (pid == 0)
			handle_phi(&phi[i]);
		else
			handle_parent(phi, i, &pid_tab, pid);
		if (usleep(10) < 0)
			return (EXIT_FAILURE);
	}
	return (0);
}

int			main(int ac, char **av)
{
	t_phi			*phi;
	sem_t			*forks_sem;
	sem_t			*wr_sem;
	int				*pid_tab;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE ||
		init_tabs(&phi, &forks_sem, &wr_sem))
		return (EXIT_FAILURE);
	if (!(pid_tab = malloc(sizeof(int) * phi[0].total)))
		return (EXIT_FAILURE);
	if (get_time(&phi[0]) < 0 || launch_proc(phi, pid_tab))
		return (EXIT_FAILURE);
	return (free_all(phi, pid_tab));
}

/*
**			Fin du programme : Si un philosophe meurt, ou si tous les
**			philosophes ont mangé number_of_time_each_philosophers_must_eat fois
**			(s'il est défini, sinon l'init à -1).
*/
