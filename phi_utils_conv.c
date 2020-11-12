/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phi_utils_conv.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/01 14:50:42 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/12 12:04:57 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static char		*rev_tab(char *tab)
{
	int		i;
	size_t	j;
	size_t	len;
	char	stock;

	i = 0;
	len = ft_strlen(tab);
	j = len - 1;
	while (i < (ft_strlen(tab) / 2))
	{
		stock = tab[i];
		tab[i] = tab[j];
		tab[j] = stock;
		i++;
		j--;
	}
	tab[len] = '\0';
	return (tab);
}

static char		*init_tab_itoa(int quot, long long n, int minus)
{
	char		*tab;
	int			i;
	long long	j;

	j = n;
	if (n < 0)
		j *= -1;
	tab = NULL;
	i = 0;
	if (!(tab = malloc(sizeof(char) * (quot + 2))))
		return (NULL);
	while (j > 9)
	{
		tab[i++] = (j % 10) + 48;
		j = j / 10;
	}
	tab[i] = j + 48;
	if (minus < 0)
		tab[++i] = '-';
	tab[i + 1] = '\0';
	return (rev_tab(tab));
}

char			*ft_itoa(long long n)
{
	char		*result;
	long long	i;
	int			quot;
	int			minus;

	i = n;
	quot = 0;
	minus = 1;
	if (n < 0)
	{
		minus *= -1;
		i *= -1;
		quot += 1;
	}
	while (i > 9)
	{
		quot++;
		i = i / 10;
	}
	result = init_tab_itoa(quot, n, minus);
	return (result);
}

int	ft_atoi(char *str)
{
	long long	res;
	int 		i;

	res = 0;
	if (!str || !str[0])
		return (-1);
	i = -1;
	while (str[++i])
	{
		if (!is_num(str[i]))
			return (-1);
		res *= 10;
		res += str[i] - 48;
	}
	return (res);
}