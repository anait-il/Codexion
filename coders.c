#include "codexion.h"

void	*worker(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("the id is : %d", id);
	usleep(500000);
	return (NULL);
}

int	setup_coders(t_args data, t_coder *coders[])
{
	pthread_t	t[data.number_of_coders];
	int			i;
	int			status;

	i = 0;
	while (i < data.number_of_coders)
	{
		coders[i]->id = i;
		status = pthread_create(&t[i], NULL, worker, &coders[i]->id);
		if (status)
			return (1);
		coders[i]->thread = t[i];
		i++;
	}
	return (0);
}

int	join_coders(t_args data, t_coder *coders[])
{
	int	y;

	y = 0;
	while (y < data.number_of_coders)
	{
		pthread_join(coders[y]->thread, NULL);
		y++;
	}
	return (0);
}
