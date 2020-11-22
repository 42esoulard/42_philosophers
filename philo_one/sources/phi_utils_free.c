/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:53:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/22 13:45:23 by esoulard         ###   ########.fr       */
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

int		free_all(t_phi *phi, pthread_t *thread_tab)
{
	int	i;

	free(thread_tab);
	i = -1;
	while (++i < phi[0].total)
		pthread_mutex_destroy(&((*(phi[0].mutex))[i]));
	free(*(phi[0].mutex));
	free(*(phi[0].fork));
	free(phi[0].end);
	pthread_mutex_destroy(phi[0].wr_mutex);
	free(phi[0].wr_mutex);
	free_phi(phi);
	return (0);
}
