/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/09 11:35:40 by esoulard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"
#include <stdio.h>


// int 	ft_strerror(char *str, t_philo **philo)
// {
// 	write (2, str, ft_strlen(str));
// 	free(*philo);
// 	return (-1)
// }

int 	free_phi(t_phi *phi)
{
	if (phi)
		free(phi);
	return (-1);
}

int		update_last_meal(t_phi **phi)
{
	// if (!((phi->tv) = (struct timeval *)malloc(sizeof((phi->tv)))))
	// 	return (-1);
	get_time(phi);
	//printf("bef loast meal\n");
	(*phi)->last_meal = (*phi)->time;
	//usleep(10);
	//printf("aft loast meal\n");
	// free(phi->tv);
	// phi->tv = NULL;
	return (0);
}

void	action_msg(t_phi *phi, char *action)
{ 
	char	*tmpa;
	char	*tmpb;
	char	*to_print;
	int		len;
	int		i;

	// printf("0\n");
	tmpa = ft_itoa(phi->cur);
	// printf("1\n");
	tmpb = ft_itoa(phi->time - phi->start);
	// printf("2\n");
	len = ft_strlen(tmpa) + ft_strlen(tmpb) + ft_strlen(action);
	if (!(to_print = malloc(sizeof(char) * (len + 4))))
		exit(EXIT_FAILURE);
	// printf("3\n");
	i = -1;
	while (tmpa[++i])
		to_print[i] = tmpa[i];
	to_print[i++] = ' ';
	len = 0;
	while (tmpb[len])
		to_print[i++] = tmpb[len++];
	to_print[i++] = ' ';
	len = 0;
	while (action[len])
		to_print[i++] = action[len++];
	to_print[i++] = '\n';
	to_print[i] = '\0';
	ft_putstr(0, to_print);
	free(tmpa);
	free(tmpb);
	free(to_print);
}
//LES FOURCHETTES SONT NUMEROTEES EN DECALE A GAUCHE DE CHAQUE PHILO
//EX: Phi 7 à à sa gauche la fork7 et a droite la fork8 (OU 0 SIL EST LE DERNIER)
int 	get_time(t_phi **phi)
{
	gettimeofday(&((*phi)->tv), NULL);
	(*phi)->time = (*phi)->tv.tv_sec * 1000 + (*phi)->tv.tv_usec / 1000;
	return (0);
}

int		is_dead(t_phi **phi)
{
	get_time(phi);
	//printf("phi %d, t %lld, last meal %ld, t_die %ld\n", phi->cur, phi->time, phi->last_meal, phi->t_die);
	//getchar();
	if (((*phi)->time - (*phi)->last_meal)
		>= (*phi)->t_die)
	{
		action_msg(*phi, "died");
		//phi->status = DEAD;
		exit(0);
	}
	//printf("phi %d last meal %lld, time since %lld\n", (*phi)->cur, (*phi)->last_meal, ((*phi)->time - (*phi)->last_meal));
	// free(phi->tv);
	// phi->tv = NULL;
	return (0);
}

void		go_eat(t_phi *tmp)
{
	while (((*tmp->fork)[tmp->fork_a] == NOT_AVAIL
		|| (*tmp->fork)[tmp->fork_b] == NOT_AVAIL)
		&& is_dead(&tmp) == 0);
	if (is_dead(&tmp) == 0)
	{
		get_time(&tmp);
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_a]);
		(*tmp->fork)[tmp->fork_a] = NOT_AVAIL;
		action_msg(tmp, "has taken a fork");
		get_time(&tmp);
		pthread_mutex_lock(&(*tmp->mutex)[tmp->fork_b]);
		(*tmp->fork)[tmp->fork_b] = NOT_AVAIL;
		action_msg(tmp, "has taken a fork");
		update_last_meal(&tmp);
		action_msg(tmp, "is eating");
		usleep(tmp->t_eat * 1000);
		(*tmp->fork)[tmp->fork_a] = AVAIL;
		(*tmp->fork)[tmp->fork_b] = AVAIL;
		pthread_mutex_unlock(&(*tmp->mutex)[tmp->fork_a]);
		pthread_mutex_unlock(&(*tmp->mutex)[tmp->fork_b]);
	}
	tmp->status++;
}

void		*handle_phi(void *phi)
{
	t_phi *tmp;

	tmp = (t_phi *)phi;
	update_last_meal(&tmp);
	while (is_dead(&tmp) == 0)
	{
		if (tmp->status == EATS)
			go_eat(tmp);
		if (tmp->status == SLEEPS && is_dead(&tmp) == 0)
		{
			action_msg(tmp, "is sleeping");
			usleep(tmp->t_sleep * 1000);
			tmp->status++;
		}
		if (tmp->status == THINKS && is_dead(&tmp) == 0)
		{
			action_msg(tmp, "is thinking");
			while (((*tmp->fork)[tmp->fork_a] == NOT_AVAIL
				|| (*tmp->fork)[tmp->fork_b] == NOT_AVAIL)
				&& is_dead(&tmp) == 0)
				usleep(10);
			if ((tmp->time - tmp->last_meal) < (tmp->t_die))
				usleep((tmp->t_die - (tmp->time - tmp->last_meal)) / (tmp->total) * 100);
			tmp->status = EATS;
		}
	}
	return (NULL);
}

int 	launch_threads(int i, t_phi *phi, pthread_t *thread_tab)
{
	while (i < phi[0].total)
	{
		thread_tab[i] = 0;
		phi[i].start = phi[0].time;
		if (pthread_create(&thread_tab[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
		usleep(50);
		i += 2;
	}
	i = -1;
	while (++i < phi[0].total)
		if (pthread_join(thread_tab[i], NULL))
			return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	main(int ac, char **av)
{
	int				*fork;
	pthread_mutex_t *mutex;
	t_phi			*phi;
	pthread_t		*thread_tab;
	int				i;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE)//PARSING
		return (EXIT_FAILURE);
	if (!(fork = malloc(sizeof(int) * phi[0].fork_total)))
		return (EXIT_FAILURE);
	if (!(mutex = malloc(sizeof(pthread_mutex_t) * phi[0].fork_total)))
		return (EXIT_FAILURE);
	if (!(thread_tab = malloc(sizeof(pthread_t) * phi[0].total)))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < phi[0].fork_total)
	{
		fork[i] = AVAIL;
		if (pthread_mutex_init(&mutex[i], NULL) != 0)
			return (EXIT_FAILURE);
		phi[i].fork = &fork;
	 	phi[i].mutex = &mutex;
	}
	get_time(&phi);
	if (launch_threads(0, phi, thread_tab) == EXIT_FAILURE ||
		launch_threads(1, phi, thread_tab) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	free(thread_tab);
	free(*(phi[0].fork));
	free(*(phi[0].mutex));
	free_phi(phi);
	return (0);
}
/*
			Fin du programme : Si un philosophe meurt, ou si tous les philosophes ont mangé number_of_time_each_philosophers_must_eat fois (s'il est défini, sinon l'init à -1).
			Any of the threads in the process calls exit(3), or the main thread performs a return from main(). This causes the termination of all threads in the process.
			Il me faut comme fonctions :
					handle_philo
*/
	
