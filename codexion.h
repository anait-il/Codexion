#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct s_args
{
	int			number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	int			number_of_compiles_required;
	long long	dongle_cooldown;
	char		*scheduler;
}				t_args;

typedef struct s_coder
{
	int			id;
	pthread_t	thread;
}				t_coder;

typedef struct s_dongle
{
    bool        inuse;
    long		userid;
    int         id;
}               t_dongle;

long long	ft_atoi(char *str);
int			parsing(int ac, char *av[]);
int			is_valid(char *av[]);
int			ft_isdigit(int i);
int	setup_coders(t_args data, t_coder *coders[]);
int	join_coders(t_args data, t_coder* coders[]);
