/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:53:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/27 13:20:24 by esoulard         ###   ########.fr       */
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

int		free_all(t_phi *phi, pthread_t *c_thr, pthread_t *p_thr)
{
	int i;

	i = -1;
	while (++i < phi[0].total)
		sem_close(phi[i].eat_sem);
	if (phi[0].total == 1)
		sem_post(phi[0].forks_sem[0]);
	sem_close(*(phi[0].forks_sem));
	sem_close(*(phi[0].wr_sem));
	free(phi[0].end);
	free_phi(phi);
	free(p_thr);
	free(c_thr);
	return (0);
}
