#include "codexion.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t	my_mutex;

int				program_count = 0;

void	*worker(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("the custom number is: %d\n", id);
	fflush(stdout);
	pthread_mutex_lock(&my_mutex);
	program_count += 1;
	pthread_mutex_unlock(&my_mutex);
	return (NULL);
}

int	main(void)
{
	pthread_t	t[10];
	int			i;
	int			tmp[10];

	pthread_mutex_init(&my_mutex, NULL);
	i = 0;
	while (i < 10)
	{
		tmp[i] = i;
		pthread_create(&t[i], NULL, worker, &tmp[i]);
		i++;
	}
	i = 0;
	while (i < 10)
	{
		pthread_join(t[i], NULL);
		i++;
	}
	printf("count = %d", program_count);
	return (0);
}
