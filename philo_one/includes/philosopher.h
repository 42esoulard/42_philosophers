/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:54 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/24 10:27:28 by esoulard         ###   ########.fr       */
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

# define EATS 0
# define SLEEPS 1
# define THINKS 2
# define DEAD 3

typedef struct		s_phi
{
	int				total;
	int				cur;
	int				status;
	long int		t_die;
	long int		t_eat;
	long int		t_sleep;
	int				nb_meals;
	int				ct_meals;
	pthread_mutex_t	**fk_mx;
	pthread_mutex_t *wr_mx;
	pthread_mutex_t *eat_mx;
	int				*end;
	int				fork_a;
	int				fork_b;
	long long		last_meal;
	long long		time;
	long long		start;
	struct timeval	tv;
	int				tmp;
}					t_phi;

int					init_phi(int ac, char **av, t_phi **phi);
int					init_tabs(t_phi **phi, pthread_mutex_t **fk_mx,
	pthread_mutex_t **eat_mx);
int					ft_init_err(char *stra, char *strb, t_phi **phi);

void				*handle_phi(void *phi);
long long			get_time(t_phi *phi);
int					nap_time(t_phi *phi, int duration);
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
int					free_all(t_phi *phi, pthread_t *c_thr, pthread_t *p_thr);

#endif

/*
**typedef struct		s_phi
**{
**	int				total;//nb of philosophers
**	int				cur;//index of current phi
**	long long		last_meal;//timestamp of cur_phi's last meal
**	int				status;//EATS, SLEEPS, THINKS or DEAD
**	long int		t_die;//time_to_die in ms
**	long int		t_eat;//time_to_eat in ms
**	long int		t_sleep;//time_to_sleep in ms
**	int				nb_meals;//nb of meals each phi must eat. if none, -1
**	int				ct_meals;//nb of meals eaten;
**	pthread_mutex_t	**mutex;//mutex tab
**	pthread_mutex_t *wr_mutex;
**	int				*wr_check;
**	int				**fork;//fork boolean tab
**	int				*end;//0 = not over, 1 = dead phi
**	int				fork_a;//first fork, = cur
**	int				fork_b;//second fork, = cur + 1 (or 0 if last)
**	struct timeval	tv;// to store gettimeofday return
**	long long		time;//last timestamp recorded
**	long long		start;//timestamp of prog start
**	int				tmp;//random counter
**}					t_phi;
*/
