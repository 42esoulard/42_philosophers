/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: esoulard <esoulard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 11:29:38 by esoulard          #+#    #+#             */
/*   Updated: 2020/11/05 17:34:42 by esoulard         ###   ########.fr       */
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

int 	ft_init_err(char *stra, char *strb, t_phi **phi)
{
	if (stra)
		ft_putstr(2, stra);
	if (strb)
	{
		ft_putstr(2, ": invalid parameter.\n\n");
		ft_putstr(2, strb);
		ft_putstr(2, " should be a positive integer.\n");
	}
	ft_putstr(2, "\nTry: ./philosopher <number_of_philosopher> ");
	ft_putstr(2, "<time_to_die> <time_to_eat> <time_to_sleep> ");
	ft_putstr(2, "[number_of_time_each_philosophers_must_eat]\n");
	if (phi)
		return (free_phi(*phi));
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

void		*handle_phi(void *phi)
{
	// (void)phi;
	t_phi *tmp;

	tmp = (t_phi *)phi;
	//printf("in threads phi->cur %d status %d\n", tmp->cur, tmp->status);
	update_last_meal(&tmp);
	while (is_dead(&tmp) == 0)
	{
		// printf("1 stat %d\n", tmp->status);
		if (tmp->status == EATS)
		{
			//printf("%d in EAT\n", tmp->cur);
			while (((*tmp->fork)[tmp->fork_a] == NOT_AVAIL
				|| (*tmp->fork)[tmp->fork_b] == NOT_AVAIL)
				&& is_dead(&tmp) == 0);
			// printf("3\n");
			if (is_dead(&tmp) == 0)
			{
				//printf("BEGINNING OF MEAL f_a %d f_b %d\n", tmp->fork_a, tmp->fork_b);
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
			// if ((tmp->time - tmp->last_meal) < (tmp->t_die / 4))
			// 	usleep(tmp->t_die / 4 * 1000);
		}
		// printf("4\n");
		if (tmp->status == SLEEPS && is_dead(&tmp) == 0)
		{
			//printf("%d SLEEPS\n", tmp->cur);
			action_msg(tmp, "is sleeping");
			usleep(tmp->t_sleep * 1000);
			tmp->status++;
		}
		if (tmp->status == THINKS && is_dead(&tmp) == 0)
		{
			action_msg(tmp, "is thinking");
			
			// if ((tmp->time - tmp->last_meal)
			// < (tmp->t_die / 10))
			
			while (((*tmp->fork)[tmp->fork_a] == NOT_AVAIL
				|| (*tmp->fork)[tmp->fork_b] == NOT_AVAIL)
				&& is_dead(&tmp) == 0)
				usleep(10);
			//get_time(&tmp);
			if ((tmp->time - tmp->last_meal) < (tmp->t_die))
				usleep((tmp->t_die - (tmp->time - tmp->last_meal)) / (tmp->total) * 100);
			// printf("bef\n");
			// usleep((tmp->t_die - (tmp->time - tmp->last_meal) * 1000)/4);
			// printf("aft\n");
				// printf("-----AM STUCK\n");
				//getchar();
			tmp->status = EATS;
		}
		// printf("5\n");
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	int				*fork;
	pthread_mutex_t *mutex;
	t_phi			*phi;
	pthread_t		*thread_tab;
	int				i;
	int 			fork_nb;
	long 			start_time;

	if (init_phi(ac, av, &phi) == EXIT_FAILURE)//PARSING
		return (EXIT_FAILURE);
	fork_nb = phi[0].total;
	if (fork_nb == 1)
		fork_nb++;
	if (!(fork = malloc(sizeof(int) * fork_nb)))
		return (EXIT_FAILURE);
	if (!(mutex = malloc(sizeof(pthread_mutex_t) * fork_nb)))
		return (EXIT_FAILURE);
	if (!(thread_tab = malloc(sizeof(pthread_t) * phi[0].total)))
		return (EXIT_FAILURE);
	i = -1;
	while (++i < fork_nb)
	{
		//printf("init fork + mutex %d\n", i);
		fork[i] = AVAIL;
		if (pthread_mutex_init(&mutex[i], NULL) != 0)
			exit (EXIT_FAILURE);
	}
	i = 0;
	get_time(&phi);
	while (i < phi[0].total)
	{
		//thread_tab[i] = 0;
		phi[i].fork = &fork;
		phi[i].mutex = &mutex;
		phi[i].start = phi[0].time;
		//printf("in main phi->cur %d\n", phi[i].cur);
		if (pthread_create(&thread_tab[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
		usleep(50);
		i += 2;
	}
	i = 1;
	while (i < phi[0].total)
	{
		//thread_tab[i] = 0;
		phi[i].fork = &fork;
		phi[i].mutex = &mutex;
		phi[i].start = phi[0].time;
		//printf("in main phi->cur %d\n", phi[i].cur);
		if (pthread_create(&thread_tab[i], NULL, handle_phi, &phi[i]))
			return (EXIT_FAILURE);
		usleep(50);
		i += 2;
	}
	//printf("after loop\n");
	i = -1;
	while (++i < phi[0].total)
		if (pthread_join(thread_tab[i], NULL))
			return (EXIT_FAILURE);
	//printf("after join\n");
	free(thread_tab);
	free(fork);
	free_phi(phi);
	return (0);
}
/*
			Fin du programme : Si un philosophe meurt, ou si tous les philosophes ont mangé number_of_time_each_philosophers_must_eat fois (s'il est défini, sinon l'init à -1).
			Any of the threads in the process calls exit(3), or the main thread performs a return from main(). This causes the termination of all threads in the process.
			Il me faut comme fonctions :
					handle_philo
*/
	
