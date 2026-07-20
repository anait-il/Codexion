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
	struct s_program	*program;
	int					compile_counter;
	long				last_compile_time;
	struct s_dongle		*left;
	struct s_dongle		*right;
	long				arrival_time;
	long				deadline;
}						t_coder;

typedef struct s_dongle
{
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
	pthread_mutex_t		monitor_lock;
	pthread_mutex_t		print_lock;
}						t_program;

long long				ft_atoi(char *str);
int						parsing(int ac, char *av[], t_program *program);
int						is_valid(char *av[]);
int						ft_isdigit(int i);
int						setup_dongles(t_program *program);
int						setup_coders(t_program *program);
int						join_coders(t_program *program);
int						clean_up(t_program *program);
int						clean_threads(t_program *program, int coders_counter);
int						simulation(t_coder *coder);
int						all_thread_ready(t_program *program);
long					get_time_ms(void);
long					get_elapsed_ms(long start);
int						heap_push(t_heap *heap, t_coder *coder);
t_coder					*heap_pop(t_heap *heap);
t_coder					*heap_top(t_heap *heap);
int                     acquire_dongles(t_coder *coder);
void                    release_dongles(t_coder *coder);
void                    assign_dongles(t_coder *coder, t_program *program, int counter);
int                     log_state(t_coder   *coder, char *message);
void                    log_burnout(t_program *program, int coder_id);
void                    free_dongles(t_program *program);
void                    destroy_mtx_cond(t_program *program);
int                     start_monitoring(t_program *program);
void                    stop_simulation(t_program *program);
void					my_sleep(long time, t_program *program);
bool					is_running(t_program *program);