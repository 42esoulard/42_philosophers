/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_str.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:54:04 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/01 14:56:25 by esoulard         ###   ########.fr       */
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
	write(out, str, ft_strlen(str));
	return (0);
}