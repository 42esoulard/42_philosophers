/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:53:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/23 18:27:02 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int		free_strs(char *stra, char *strb, char *strc)
{
	if (stra)
		free(stra);
	if (strb)
		free(strb);
	if (strc)
		free(strc);
	return (EXIT_SUCCESS);
}

int		free_phi(t_phi *phi)
{
	if (phi)
		free(phi);
	return (EXIT_FAILURE);
}

int		free_all(t_phi *phi, int *pid_tab)
{
	int i;

	i = -1;
	while (++i < phi[0].total)
		sem_close(phi[i].eat_sem);
	sem_close(*(phi[0].forks_sem));
	sem_close(*(phi[0].wr_sem));
	free(pid_tab);
	free_phi(phi);
	return (0);
}
