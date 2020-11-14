/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:54 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/14 12:19:01 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>

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
	int				total;//nb of philosophers
	int				cur;//index of current phi
	long long		last_meal;//timestamp of cur_phi's last meal 
	int				status;//EATS, SLEEPS, THINKS or DEAD
	long int		t_die;//time_to_die in ms
	long int		t_eat;//time_to_eat in ms
	long int		t_sleep;//time_to_sleep in ms
	int				nb_meals;//nb of meals each phi must eat before stopping. if none, -1
	int 			ct_meals;//nb of meals eaten;
	pthread_mutex_t	**mutex;//mutex tab
	int				**fork;//fork boolean tab
	int				*end;//0 = not over, 1 = dead phi
	int				fork_a;//first fork, = cur
	int				fork_b;//second fork, = cur + 1 (or 0 if last)
	int				fork_total;//MUST BE DELETED, ONLY USEFUL IF 1 PHI
	struct timeval	tv;// to store gettimeofday return
	long long		time;//last timestamp recorded
	long long		start;//timestamp of prog start
	int				tmp;//random counter
}					t_phi;


int					init_phi(int ac, char **av, t_phi **phi);

int 				ft_init_err(char *stra, char *strb, t_phi **phi);


void				*handle_phi(void *phi);
int 				get_time(t_phi *phi);
long				forecast(t_phi *tmp, long action_time);
int					go_eat(t_phi **tmp);
int					update_last_meal(t_phi **phi);
int					is_dead(t_phi **phi);

int					action_msg(t_phi *phi, char *action);
int					ft_strlen(char *str);
int					is_num(char a);
int					ft_putstr(int out, char *str);

char				*ft_itoa(long long n);
int					ft_atoi(char *str);

int					free_strs(char *stra, char *strb, char *strc);
int					free_phi(t_phi *phi);
int					free_all(t_phi *phi, pthread_t *thread_tab);



#endif
