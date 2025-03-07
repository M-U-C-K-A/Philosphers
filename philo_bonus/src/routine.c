/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:12:39 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 13:04:21 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	print_status(t_philo *philo, char *status)
{
	sem_wait(philo->rules->write_lock);
	if (!alive_state(philo) && ft_strcmp(status, "died"))
	{
		sem_post(philo->rules->write_lock);
		return ;
	}
	printf("%5lli " "%3d " "%s\n", get_timestamp()
		- philo->rules->start_time, philo->id, status);
	sem_post(philo->rules->write_lock);
}

void	eating(t_philo *philo)
{
	sem_wait(philo->rules->fork);
	print_status(philo, "has taken a fork");
	if (philo->rules->nb_philo == 1)
	{
		sem_post(philo->rules->fork);
		return ;
	}
	sem_wait(philo->rules->fork);
	print_status(philo, "has taken a fork");
	print_status(philo, "is eating");
	philo->last_meal = get_timestamp();
	philo->time_eaten++;
	usleep(philo->rules->time_to_eat * 1000);
	sem_post(philo->rules->fork);
	sem_post(philo->rules->fork);
}

void	philo_routine(t_philo *philo, t_rules *rules)
{
	if (philo->id % 2 == 0)
		usleep(500);
	while (alive_state(philo))
	{
		eating(philo);
		if (philo->rules->nb_philo == 1)
		{
			usleep(philo->rules->time_to_die * 1000);
			break ;
		}
		if (philo->time_eaten == philo->rules->number_of_meal)
			return ;
		print_status(philo, "is sleeping");
		usleep(philo->rules->time_to_sleep * 1000);
		print_status(philo, "is thinking");
	}
	print_status(philo, "died");
	free_all(rules);
	exit (EXIT_SUCCESS);
}

void	end_process(t_rules *rules, pid_t death_check)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		if (rules->wait_philo[i] != death_check)
			kill(rules->wait_philo[i], SIGTERM);
		i++;
	}
	i = 0;
	while (i < rules->nb_philo)
	{
		waitpid(rules->wait_philo[i], NULL, 0);
		i++;
	}
}

void	create_philo(t_rules *rules)
{
	int		i;
	pid_t	pid;
	pid_t	death_check;

	i = 0;
	while (i < rules->nb_philo)
	{
		pid = fork();
		if (pid == -1)
			exit (exit_print("Fork error"));
		if (pid == 0)
			philo_routine(&rules->philo[i], rules);
		rules->wait_philo[i] = pid;
		i++;
	}
	death_check = waitpid(-1, NULL, 0);
	if (death_check > 0)
		end_process(rules, death_check);
}
