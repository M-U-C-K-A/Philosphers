/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_state.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbardet- <rbardet-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 04:55:37 by rbardet-          #+#    #+#             */
/*   Updated: 2025/03/07 18:07:50 by rbardet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo_bonus.h"

int	alive_state(t_philo *philo)
{
	sem_wait(philo->rules->state_lock);
	if (((get_timestamp() - philo->last_meal) > philo->rules->time_to_die))
	{
		printf("%5lli " "%3d " "is dead\n", get_timestamp()
			- philo->rules->start_time, philo->id);
		sem_post(philo->rules->state_lock);
		exit (EXIT_SUCCESS);
	}
	sem_post(philo->rules->state_lock);
	return (1);
}
