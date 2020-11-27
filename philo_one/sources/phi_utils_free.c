/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:53:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/27 11:23:05 by esoulard         ###   ########.fr       */
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
	int	i;

	pthread_mutex_destroy(phi[0].wr_mx);
	free(phi[0].wr_mx);
	free(phi[0].end);
	free(p_thr);
	free(c_thr);
	i = -1;
	while (++i < phi[0].total)
	{
		pthread_mutex_destroy(&((phi[0].eat_mx)[i]));
		pthread_mutex_destroy(&((*(phi[0].fk_mx))[i]));
	}
	free(phi[0].eat_mx);
	free(*(phi[0].fk_mx));
	free_phi(phi);
	system("leaks ./philo_one");
	return (0);
}
