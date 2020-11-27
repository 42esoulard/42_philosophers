/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:54:04 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/27 11:25:13 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int			ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int			is_num(char a)
{
	if (a >= 48 && a <= 57)
		return (1);
	return (0);
}

int			ft_putstr(int out, char *str)
{
	if (write(out, str, ft_strlen(str)) < 0)
		return (EXIT_FAILURE);
	return (0);
}

static char	*phi_strjoin(char *time, char *index, char *action)
{
	char	*to_print;
	int		i;
	int		j;

	if (!(to_print = malloc(sizeof(char) * (ft_strlen(time) +
		ft_strlen(index) + ft_strlen(action) + 4))))
		return (NULL);
	i = -1;
	while (time[++i])
		to_print[i] = time[i];
	to_print[i++] = ' ';
	j = 0;
	while (index[j])
		to_print[i++] = index[j++];
	to_print[i++] = ' ';
	j = 0;
	while (action[j])
		to_print[i++] = action[j++];
	to_print[i++] = '\n';
	to_print[i] = '\0';
	free_strs(time, index, NULL);
	return (to_print);
}
#include <stdio.h>
int			action_msg(t_phi *phi, char *action)
{
	char	*time;
	char	*index;
	char	*to_print;

	if (*(phi->end) == DEAD)
		return (EXIT_FAILURE);
	if (pthread_mutex_lock((phi->wr_mx)) != 0)
		return (EXIT_FAILURE);
	if (!(time = ft_itoa(phi->time - phi->start)) ||
		!(index = ft_itoa(phi->cur)) ||
		!(to_print = phi_strjoin(time, index, action)))
	{
		pthread_mutex_unlock(phi->wr_mx);
		return (EXIT_FAILURE);
	}
	if (*(phi->end) != DEAD && ft_putstr(0, to_print))
	{
		pthread_mutex_unlock(phi->wr_mx);
		return (EXIT_FAILURE);
	}
	free(to_print);
	if (pthread_mutex_unlock((phi->wr_mx)) != 0)
		return (EXIT_FAILURE);
	// printf("out str\n");
	return (EXIT_SUCCESS);
}
