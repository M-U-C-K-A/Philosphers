/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:23:14 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 11:06:38 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

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

void	free_all(t_rules *rules)
{
	sem_close(rules->fork);
	sem_close(rules->dead_lock);
	sem_close(rules->write_lock);
	sem_close(rules->state_lock);
	sem_unlink("/fork");
	sem_unlink("/dead");
	sem_unlink("/write");
	sem_unlink("/state");
	free(rules->philo);
	free(rules);
}

int	main(int argc, char **argv)
{
	t_rules	*rules;

	if (argc < 5 || argc > 6)
	{
		write(2, "Wrong number of argument", 25);
		return (1);
	}
	rules = init_rules(argc, argv);
	rules->is_dead = 0;
	rules->first_dead = 0;
	if (rules->check != 0)
		return (1);
	if (check_rules(rules) == 0)
	{
		write(2, "At least one rule is not valid", 30);
		return (1);
	}
	init_philo(rules);
	if (!rules)
		exit (EXIT_FAILURE);
	create_thread(rules);
	free_all(rules);
	return (0);
}
