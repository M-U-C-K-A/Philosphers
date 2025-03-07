/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 03:12:39 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 22:26:20 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

void	think_routine(t_philo *philo, int silent)
{
	long long	time_to_think;

	sem_wait(philo->rules->state_lock);
	time_to_think = (philo->rules->time_to_die
			- (get_timestamp() - philo->last_meal)
			- philo->rules->time_to_eat) / 2;
	sem_post(philo->rules->state_lock);
	if (time_to_think < 0)
		time_to_think = 0;
	if (silent && time_to_think == 0)
		time_to_think = 1;
	if (time_to_think > 600)
		time_to_think = 200;
	if (!silent)
		print_status(philo, "is thinking");
	usleep(time_to_think * 1000);
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
	sem_wait(philo->rules->state_lock);
	philo->last_meal = get_timestamp();
	sem_post(philo->rules->state_lock);
	philo->time_eaten++;
	better_usleep(philo->rules->time_to_eat, philo);
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
		{
			free_all(rules);
			return ;
		}
		print_status(philo, "is sleeping");
		better_usleep(philo->rules->time_to_sleep, philo);
		think_routine(philo, 1);
	}
	print_status(philo, "died");
	free_all(rules);
	exit(EXIT_SUCCESS);
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
