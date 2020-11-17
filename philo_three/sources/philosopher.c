/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/17 12:12:26 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		launch_proc(t_phi *phi, int *pid_tab)
{
	int i;
	int pid;
	int ret;
	int status;

	i = -1;
	ret = EXIT_SUCCESS;

	while (++i < phi[0].total)
	{
		// printf("launching i %d\n", i);
		phi[i].start = phi[0].time;
		phi[i].last_meal = phi[0].time;
		if ((pid = fork()) < 0)
		{
			ft_putstr(2, "fork failed\n");
			return (EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			// printf("in child i %d\n", i);
			handle_phi(&phi[i]);
		}
		else
		{
			pid_tab[i] = pid;
			if (i + 1 == phi[0].total)
			{
				while (waitpid(-1, &status, 0) != -1)
				{
					if (WIFEXITED(status))
						ret = WEXITSTATUS(status);
					if (ret != 0)
					{
						i = -1;
						while (++i < phi[0].total)
						{
							printf("kill pid %d\n", pid_tab[i]);
							kill(pid_tab[i], SIGINT);
						}
					}
				}
			}
		}
		usleep(10);
	}
	return (ret);
}

int		main(int ac, char **av)
{
	t_phi			*phi;
	sem_t			*forks_sem;
	sem_t			*wr_sem;
	// sem_t			*end_sem;
	int				*pid_tab;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE ||
		init_tabs(&phi, &forks_sem, &wr_sem))
		return (EXIT_FAILURE);
	if (!(pid_tab = malloc(sizeof(int) * phi[0].total)))
		return (EXIT_FAILURE);
	if (get_time(&phi[0]) || launch_proc(phi, pid_tab))
		return (EXIT_FAILURE);
	return (free_all(phi, pid_tab));
}

/*
**			Fin du programme : Si un philosophe meurt, ou si tous les
**			philosophes ont mangé number_of_time_each_philosophers_must_eat fois
**			(s'il est défini, sinon l'init à -1).
*/
