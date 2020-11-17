/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:54:04 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/17 12:37:52 by esoulard         ###   ########.fr       */
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
	return (to_print);
}

int			action_msg(t_phi *phi, char *action)
{
	char	*time;
	char	*index;
	char	*to_print;

	if (sem_wait(*(phi->wr_sem)))
		return (EXIT_FAILURE);
	if (!(time = ft_itoa(phi->time - phi->start)) ||
		!(index = ft_itoa(phi->cur)) ||
		!(to_print = phi_strjoin(time, index, action)))
		return (EXIT_FAILURE);
	if (ft_putstr(0, to_print))
		return (EXIT_FAILURE);
	if (sem_post(*(phi->wr_sem)))
		return (EXIT_FAILURE);
	return (free_strs(time, index, to_print));
}
