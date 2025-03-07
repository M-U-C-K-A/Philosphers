/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:23:14 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 22:03:53 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_timestamp(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "Error while getting actual time\n", 33);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	check_rules(t_rules *rules)
{
	if (rules->nb_philo < 0 || rules->time_to_die < 0
		|| rules->time_to_eat < 0 || rules->time_to_eat < 0
		|| rules->time_to_sleep < 0)
	{
		return (0);
	}
	else
		return (1);
}

void	free_all(pthread_mutex_t **fork, t_rules *rules)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&rules->write_lock);
	pthread_mutex_destroy(&rules->dead_lock);
	pthread_mutex_destroy(&rules->state_lock);
	while (i < rules->nb_philo)
	{
		pthread_mutex_destroy(fork[i]);
		free(fork[i]);
		i++;
	}
	free(rules->philo);
	free(rules);
	free(fork);
}

int	main(int argc, char **argv)
{
	t_rules				*rules;
	pthread_mutex_t		**fork;

	if (argc < 5 || argc > 6)
	{
		write(2, "Wrong number of argument\n", 26);
		return (1);
	}
	rules = init_rules(argc, argv);
	rules->is_dead = 0;
	rules->first_dead = 0;
	if (rules->check != 0)
		return (free(rules), 1);
	if (check_rules(rules) == 0)
	{
		free(rules);
		write(2, "At least one rule is not valid\n", 32);
		return (1);
	}
	fork = init_forks(rules->nb_philo);
	init_philo(fork, rules);
	create_thread(fork, rules);
	free_all(fork, rules);
	return (0);
}
