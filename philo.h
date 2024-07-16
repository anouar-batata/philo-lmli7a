/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alouriga <alouriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/09 01:12:27 by alouriga          #+#    #+#             */
/*   Updated: 2024/07/15 16:17:05 by alouriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_data
{
    int nb_of_ph;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int nb_of_meals;
    pthread_mutex_t *forks;
    pthread_mutex_t *print;
    pthread_mutex_t *var;
    pthread_mutex_t *flag;
    long    bg_time;
    int dead;
    int m;
}               t_data;

typedef struct s_memslot
{
	void				*addr;
	struct s_memslot	*next;
}				t_slot;

typedef struct s_memslotlist
{
	t_slot	*head;
	t_slot	*last;
}				t_slotlist;


typedef struct s_philo
{
    pthread_t       *thread;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    int             indx;
    int             nb_of_m;
    size_t             time_eat;
    int             start;
    int is_dead;
    t_data          *data;
    struct s_philo  *head;
    struct s_philo         *next;
}              t_philo;


long ft_atoi(const char *str);
int check_str(char *s);

# endif