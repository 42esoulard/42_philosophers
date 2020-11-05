/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:54 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/03 15:48:02 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

# define PARAM_TDIE "<time_to_die>"
# define PARAM_TEAT "<time_to_eat>"
# define PARAM_TSLEEP "<time_to_sleep>"
# define PARAM_NB_MEAL "[number_of_time_each_philosophers_must_eat]"
# define PARAM_NB_PHI "<number_of_philosopher>"

# define AVAIL 0
# define NOT_AVAIL 1

# define EATS 0
# define SLEEPS 1
# define THINKS 2
# define DEAD 3

typedef struct		s_phi
{
	int				total;
	int				cur;
	long long		last_meal;
	int				status;
	long int		t_die;
	long int		t_eat;
	long int		t_sleep;
	int				nb_meals;
	pthread_mutex_t	**mutex;
	int				**fork;
	int				fork_a;
	int				fork_b;
	struct timeval	tv;
	long long		time;
	long long		start;
}					t_phi;

int					get_time(t_phi **phi);

int					init_phi(int ac, char **av, t_phi **phi);

int 				ft_init_err(char *stra, char *strb, t_phi **phi);

int					ft_strlen(char *str);
int					is_num(char a);
int					ft_putstr(int out, char *str);

char				*ft_itoa(long long n);
int					ft_atoi(char *str);

#endif
