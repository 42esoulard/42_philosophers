/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 11:53:05 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/09 15:07:02 by esoulard         ###   ########.fr       */
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
	return (-1);
}

int		free_all(t_phi *phi, pthread_t *thread_tab)
{
	free(thread_tab);
	free(*(phi[0].fork));
	free(*(phi[0].mutex));
	free_phi(phi);
	return (0);
}
