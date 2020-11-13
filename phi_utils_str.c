/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:54:04 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/13 16:07:43 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int is_num(char a)
{
	if (a >= 48 && a <= 57)
		return (1);
	return (0);
}

int	ft_putstr(int out, char *str)
{
	if (write(out, str, ft_strlen(str)) < 0)
		return (EXIT_FAILURE);
	return (0);
}

int	action_msg(t_phi *phi, char *action)
{ 
	char	*tmpa;
	char	*tmpb;
	char	*to_print;
	int		len;

	if (*(phi->end) == DEAD)
		return (0);
	tmpa = ft_itoa(phi->time - phi->start);
	tmpb = ft_itoa(phi->cur);
	if (!(to_print = malloc(sizeof(char) * (ft_strlen(tmpa) +
		ft_strlen(tmpb) + ft_strlen(action) + 4))))
		return (EXIT_FAILURE);
	phi->tmp = -1;
	while (tmpa[++(phi->tmp)])
		to_print[phi->tmp] = tmpa[phi->tmp];
	to_print[(phi->tmp)++] = ' ';
	len = 0;
	while (tmpb[len])
		to_print[(phi->tmp)++] = tmpb[len++];
	to_print[(phi->tmp)++] = ' ';
	len = 0;
	while (action[len])
		to_print[(phi->tmp)++] = action[len++];
	to_print[(phi->tmp)++] = '\n';
	to_print[(phi->tmp)] = '\0';
	if (ft_putstr(0, to_print))
		return (EXIT_FAILURE);
	return (free_strs(tmpa, tmpb, to_print));
}
