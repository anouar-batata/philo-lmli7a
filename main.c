/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alouriga <alouriga@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 12:50:52 by alouriga          #+#    #+#             */
/*   Updated: 2024/07/16 19:36:36 by alouriga         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = get_current_time();
	while ((get_current_time() - start) < milliseconds)
		usleep(500);
	return (0);
}

void    ft_print(t_philo *f, char *s)
{
	pthread_mutex_lock(f->data->flag);
	if (f->is_dead == 0)
	{
	pthread_mutex_unlock(f->data->flag);
	 pthread_mutex_lock(f->data->print);
	 printf("%ld %d %s", get_current_time() - f->data->bg_time, f->indx, s);
	 pthread_mutex_unlock(f->data->print);
	}
}

void    ft_sleep(t_philo *s)
{
	ft_print( s, "is sleeping\n");
	ft_usleep(s->data->time_to_sleep);
}


// ft_printf(f, "")


void    eat(t_philo *s)
{
	pthread_mutex_lock(s->left_fork);
	ft_print(s, "has taken a fork\n");
	
	pthread_mutex_lock(s->right_fork);
	ft_print(s, "has taken a fork\n");
	pthread_mutex_lock(s->data->var);
	s->time_eat = get_current_time();
	
	pthread_mutex_unlock(s->data->var);
	ft_print(s, "is eating\n");
	ft_usleep(s->data->time_to_eat);
	
	pthread_mutex_unlock(s->left_fork);
	pthread_mutex_unlock(s->right_fork);
	pthread_mutex_lock(s->data->var);
	if (s->nb_of_m > 0)
		s->nb_of_m--;
	pthread_mutex_unlock(s->data->var);
}

void    initialize_forks(int i, t_data *data)
{
	int x = 0;
	data->forks = malloc(sizeof(pthread_mutex_t) * i);
	data->print = malloc(sizeof(pthread_mutex_t));
	data->var = malloc(sizeof(pthread_mutex_t));
	data->flag = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->print, NULL);
	pthread_mutex_init(data->var, NULL);
	pthread_mutex_init(data->flag, NULL);
	while (x < i)
	{
		pthread_mutex_init(&data->forks[x], NULL); 
		x++;
	}
}


void    initialize_the_data(char **av, int ac, t_data *data)
{
	if (!data)
		return ;
	if (ac == 5)
	{
		data->nb_of_ph = ft_atoi(av[0]); 
		data->time_to_die = ft_atoi(av[1]);
		data->time_to_eat = ft_atoi(av[2]);
		data->time_to_sleep = ft_atoi(av[3]);
		data->nb_of_meals = -1;
	}
	else if (ac == 6)
	{
		data->nb_of_ph = ft_atoi(av[0]);
		data->time_to_die =ft_atoi(av[1]);
		data->time_to_eat = ft_atoi(av[2]);
		data->time_to_sleep = ft_atoi(av[3]);
		data->nb_of_meals = ft_atoi(av[4]);
	}
}



int   check_args(char **av, int ac)
{
		if ((ft_atoi(av[0]) < 0)
				|| check_str(av[0]) == -1)
		{         
			write(2, "Error", 5);
			return (-1);
		}
		else if ((ft_atoi(av[1]) <= 0|| check_str(av[1]) == -1))
		{         
			write(2, "Error", 5);
			return (-1);
		}
		else if ((ft_atoi(av[2]) <= 0 || check_str(av[2]) == -1))
		{         
			write(2, "Error", 5);
			return (-1);
		}
		else if ((ft_atoi(av[3]) <= 0 ||  check_str(av[3]) == -1))
		{         
			write(2, "Error", 5);
			return (-1);
		}
		else if (ac == 6 && (ft_atoi(av[4]) < 0 || check_str(av[4]) == -1))
		{         
			write(2, "Error", 5);
			return (-1);
		}
		return(0);
}

void    add_philo(t_philo **head, t_philo *philo)
{
	t_philo *derefrence;

	if (*head == NULL)
	{
		*head = philo;
		(*head)->head = philo;
		return ;
	}
	derefrence = *head;
	derefrence->head = derefrence;
	while (derefrence->next)
		derefrence = derefrence->next;
	derefrence->next = philo;
	*head = derefrence;
	
}

t_philo *allocate_philos(t_data *data, pthread_mutex_t *forks)
{
	int i;
	t_philo *head;
	t_philo *tmp;
	t_philo *ret;

	i = 0;
	head = NULL;
	while (i < data->nb_of_ph)
	{
		pthread_t       *thread = malloc(sizeof(pthread_t));
		tmp = malloc(sizeof(t_philo));
		tmp->indx = i + 1;
		data->bg_time = get_current_time();
		tmp->data = data;
		tmp->thread = thread;
		tmp->nb_of_m = data->nb_of_meals;
		tmp->next = NULL;
		tmp->data->dead = 0;
		tmp->data->m = 0;
		tmp->is_dead = 0;
		tmp->left_fork = &forks[(i + 1) % data->nb_of_ph];
		tmp->right_fork = &forks[i];
		tmp->time_eat = get_current_time();
		add_philo(&head, tmp);
		if (i == 0)
			ret = head;
		i++;
	}
	tmp->next = ret;
	return (ret);
}


void *start_routine(void *ptr)
{
	t_philo *philo;
	philo = (t_philo *)ptr;
	if (philo->indx % 2 == 0)
			ft_usleep(100);
	pthread_mutex_lock(philo->data->flag);
	while ((philo->nb_of_m == -1 || philo->nb_of_m > 0) && philo->data->dead == 0)
	{
		pthread_mutex_unlock(philo->data->flag);
		eat(philo);
		pthread_mutex_lock(philo->data->var);
		if (philo->nb_of_m == 0)
		{
			philo->nb_of_m = -2;
			pthread_mutex_unlock(philo->data->var);
			return (NULL);
		}
		pthread_mutex_unlock(philo->data->var);
		ft_sleep(philo);
		ft_print(philo, "is thinking\n");
	}
	return (NULL);
}
int	ft_check(t_philo *s, size_t t)
{
	int k;
	int i = 0;
	int j = 0;

	t_philo *tmp = s;
	while (s)
	{
		k = 0;
		i = 0;
		pthread_mutex_lock(s->data->var);
		if (get_current_time() - s->time_eat > s->data->time_to_die)
		{
			// pthread_mutex_unlock(s->data->var);
			
			pthread_mutex_lock(s->data->flag);
			s->data->dead = 1;
			while (j < s->data->nb_of_ph)
			{
				s->is_dead = 1;
				s = s->next;
				j++;
			}
			pthread_mutex_lock(s->data->print);
			printf("%ld %d died\n", get_current_time() - s->data->bg_time ,  s->indx);
			
			// int t = s->data->time_to_eat;
			// if (s->data->time_to_sleep)
			// 	t = s->data->time_to_eat;
			// usleep((t * 1.8) * 1000);

			return (0);
		}
		pthread_mutex_unlock(s->data->var);
		pthread_mutex_unlock(s->data->flag);
		pthread_mutex_lock(s->data->var);
		while (i < tmp->data->nb_of_ph )
		{
			if (tmp->nb_of_m == -2)
				k++;
			tmp = tmp->next;
			i++;
		}
		// pthread_mutex_unlock(s->data->var);
		// pthread_mutex_lock(s->data->var);
		if(k == s->data->nb_of_ph)
		{
			pthread_mutex_lock(s->data->print);
			return (1);
		}
		pthread_mutex_unlock(s->data->var);
		s = s->next;
	}
	return(0);
}

int     batata(char **av, int ac) 
{
	
	size_t start = get_current_time();
	t_data data;
	int  i = 0;
	int  j ;
	initialize_the_data(av, ac, &data);
	initialize_forks(ft_atoi(av[0]), &data);
	t_philo *philos = allocate_philos(&data, data.forks);
		// printf("cc\n");
	t_philo *curr;
	curr = philos;
	while(i < data.nb_of_ph)
	{
		j = pthread_create(philos->thread, NULL, &start_routine, philos); 
		pthread_detach(*philos->thread);
		philos = philos->next;
		i++;
	}
	// i = 0;
	// while (i < data.nb_of_ph)
	// {
	// 	curr = curr->next;
	// 	i++;
	// }
	i = ft_check(curr, start);
	if (i == 0)
		return (0);
	else if (i == 1)
		return (1);
	return (0);
}

void	f()
{
	system("leaks a.out");
}
int main(int ac, char *av[])
{
	// atexit(f);
	if (ac == 5 || ac == 6)
	{
		if (check_args(&av[1], ac) == -1)
			return (1);
		batata(&av[1], ac);
		// usleep(1000);
		// if (i == 0 || i == 1)
		// 	return (0);
	}
	else
	{
		write(2, "Error", 5);
		return (1);
	}
}
