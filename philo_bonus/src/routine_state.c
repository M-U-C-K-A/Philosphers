/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_state.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:55:37 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 10:25:42 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	all_alive(t_philo *philo)
{
	sem_wait(philo->rules->state_lock);
	if (*philo->is_dead == 1)
	{
		sem_post(philo->rules->state_lock);
		return (0);
	}
	else
	{
		sem_post(philo->rules->state_lock);
		return (1);
	}
}

int	alive_state(t_philo *philo)
{
	sem_wait(philo->rules->state_lock);
	if (((get_timestamp() - philo->last_meal) > philo->rules->time_to_die))
	{
		printf("philo %d : mort\n", philo->id);
		*philo->is_dead = 1;
		sem_post(philo->rules->state_lock);
		return (0);
	}
	sem_post(philo->rules->state_lock);
	return (1);
}
