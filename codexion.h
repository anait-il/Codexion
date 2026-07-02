#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>


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
	int			        id;
	pthread_t	        thread;
    struct s_program    *program;
    long                counter;
    bool                full;
}				        t_coder;
typedef struct s_dongle
{
    bool                inuse;
    long		        userid;
    int                 id;
    struct s_program    *program;
}                       t_dongle;

typedef struct s_program
{
    t_args          data;
    t_coder         *coders;
    t_dongle        *dongles;
    pthread_mutex_t my_mutex;
}                   t_program;

long long	ft_atoi(char *str);
int			parsing(int ac, char *av[], t_args *data );
int			is_valid(char *av[]);
int			ft_isdigit(int i);
int         setup_dongles(t_program *program);
int	        setup_coders(t_program program);
int	        join_coders(t_program program);
int	        clean_up(t_program program);
int	        clean_threads(t_program program, int coders_counter);
