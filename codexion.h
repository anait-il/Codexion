#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_args
{
	int					number_of_coders;
	long long			time_to_burnout;
	long long			time_to_compile;
	long long			time_to_debug;
	long long			time_to_refactor;
	int					number_of_compiles_required;
	long long			dongle_cooldown;
	char				*scheduler;
}						t_args;

typedef struct s_heap
{
	struct s_coder		**arr;
	int					size;
	int					capacity;
	struct s_program	*program;
}						t_heap;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	pthread_cond_t		sleep;
	struct s_program	*program;
	int					counter;
	long				last_compile;
	struct s_dongle		*left;
	struct s_dongle		*right;
	bool				full;
	long				arrival_time;
	long				deadline;
}						t_coder;

typedef struct s_dongle
{
	bool				inuse;
	long				available;
	int					id;
	struct s_heap		heap;
	pthread_mutex_t		lock;
	pthread_cond_t		cond;
	long				release_time;
	struct s_program	*program;
}						t_dongle;

typedef struct s_program
{
	int					running;
	pthread_t			monitor;
	long				start_time;
	t_args				data;
	t_coder				*coders;
	t_dongle			*dongles;
	pthread_mutex_t		my_mutex;
	pthread_mutex_t		print_lock;
}						t_program;

long long				ft_atoi(char *str);
int						parsing(int ac, char *av[], t_args *data);
int						is_valid(char *av[]);
int						ft_isdigit(int i);
int						setup_dongles(t_program *program);
int						setup_coders(t_program *program);
int						join_coders(t_program program);
int						clean_up(t_program program);
int						clean_threads(t_program program, int coders_counter);
int						simulation(t_coder *coder);
int						all_thread_ready(t_coder coder);
long					get_time_ms(void);
long					get_elapsed_ms(long start);
int						heap_push(t_heap *heap, t_coder *coder);
t_coder					*heap_pop(t_heap *heap);
t_coder					*heap_top(t_heap *heap);
