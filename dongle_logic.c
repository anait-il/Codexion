#include "codexion.h"

static int	can_take(t_dongle *dongle, t_coder *coder)
{
	long	now;
	
	if (!is_running(coder->program))
		return (0);
	if (!dongle->available)
		return (1);
	now = get_time_ms();
	if (now - dongle->release_time < coder->program->data.dongle_cooldown)
		return (1);
	if (heap_top(&dongle->heap) != coder)
		return (1);
	return (0);
}

static void assign_order(t_coder *coder, t_dongle *first, t_dongle *second)
{
    if (coder->id % 2 == 0)
    {
        first = coder->left;
        second = coder->right;
    }
    else
    {
        first = coder->right;
        second = coder->left;
    }
}

static int	acquire_first(t_dongle *first, t_coder *coder)
{
	int				state;
	t_coder			*status;
	long			wake_up;
	struct timespec	ts;

	pthread_mutex_lock(&first->lock);
	while (can_take(first, coder))
	{
		wake_up = first->release_time + coder->program->data.dongle_cooldown;
        ts.tv_sec = wake_up / 1000;
        ts.tv_nsec = (wake_up % 1000) * 1000000L;
        pthread_cond_timedwait(&first->cond, &first->lock, &ts);
	}
	return (0);
}

static int	acquire_second(t_dongle *first, t_dongle *second, t_coder *coder)
{
	int				state;
	t_coder			*status;
	long			wake_up;

	pthread_mutex_lock(&second->lock);
	if (can_take(second, coder))
	{
		pthread_mutex_unlock(&second->lock);
        pthread_mutex_unlock(&first->lock);
		return (1);
	}
	first->available = false;
	second->available = false;
	pthread_mutex_unlock(&second->lock);
    pthread_mutex_unlock(&first->lock);
	return (0);
}

void	setup_schedular_times(t_coder *coder)
{
	pthread_mutex_lock(&coder->program->monitor_lock);
	coder->arrival_time = get_time_ms();
	coder->deadline = coder->last_compile_time
		+ coder->program->data.time_to_burnout;
    pthread_mutex_unlock(&coder->program->monitor_lock);
}

static int	request_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	int	state;

	pthread_mutex_lock(&first->lock);
	state = heap_push(&first->heap, coder);
    pthread_mutex_unlock(&first->lock);
    if (state)
        return (1);
    pthread_mutex_lock(&second->lock);
	state = heap_push(&second->heap, coder);
    pthread_mutex_unlock(&second->lock);
    if (state)
        return (1);
	return (0);
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle		*first;
	t_dongle		*second;

    assign_order(coder, first, second);
	if (first == second)
    {
        acquire_first(first, coder);
        log_state(heap_pop(&first->heap), "has taken a dongle\n");
		return (1);
    }
    if (request_dongles(coder, first, second))
		return (1);
	while (is_running(coder->program))
	{
		acquire_first(first, coder);
		if (!acquire_second(first, second, coder))
			break;
        my_sleep(1, coder->program);
	}
    if (!is_running(coder->program))
        return (1);
	log_state(heap_pop(&first->heap), "has taken a dongle\n");
	log_state(heap_pop(&second->heap), "has taken a dongle\n");
	return (0);
}

void	release_dongles(t_coder *coder)
{
	if (!coder)
		return;
	pthread_mutex_lock(&coder->left->lock);
	coder->left->release_time = get_time_ms();
	coder->left->available = true;
	pthread_cond_broadcast(&coder->left->cond);
	pthread_mutex_unlock(&coder->left->lock);
	pthread_mutex_lock(&coder->right->lock);
	coder->right->release_time = get_time_ms();
	coder->right->available = true;
	pthread_cond_broadcast(&coder->right->cond);
	pthread_mutex_unlock(&coder->right->lock);
}