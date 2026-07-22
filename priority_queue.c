#include "codexion.h"

static void	swap(t_coder **c1, t_coder **c2)
{
	t_coder	*tmp;

	tmp = *c1;
	*c1 = *c2;
	*c2 = tmp;
}

static int	has_priority(t_coder *a, t_coder *b, char *schedular)
{
	if (strcmp(schedular, "edf") == 0)
		return (a->deadline < b->deadline);
	return (a->arrival_time < b->arrival_time);
}

static void	bubble_up(t_heap *heap, char *schedular)
{
	int	i;
	int	parent;

	i = heap->size - 1;
	parent = (i - 1) / 2;
	while (i > 0 && has_priority(heap->arr[i], heap->arr[parent], schedular))
	{
		swap(&heap->arr[i], &heap->arr[parent]);
		i = parent;
		parent = (i - 1) / 2; } }

static void	bubble_down(t_heap *heap, char *schedular)
{
	int	i;
	int	left_child;
	int	right_child;
	int	smallest;

	i = 0;
	left_child = (i * 2) + 1;
	right_child = (i * 2) + 2;
	while (left_child < heap->size)
	{
		smallest = left_child;
		if (right_child < heap->size && !has_priority(heap->arr[left_child],
				heap->arr[right_child], schedular))
			smallest = right_child;
		if (has_priority(heap->arr[i], heap->arr[smallest], schedular))
			break ;
		swap(&heap->arr[i], &heap->arr[smallest]);
		i = smallest;
		left_child = (i * 2) + 1;
		right_child = (i * 2) + 2;
	}
}

int	heap_push(t_heap *heap, t_coder *coder)
{
	t_coder	*arr;

	if (!heap || !coder || !heap->arr)
		return (1);
	if (heap->size >= heap->capacity)
	{
		fprintf(stderr, "Heap overflow\n");
		exit(1);
	}
	heap->arr[heap->size] = coder;
	heap->size++;
	bubble_up(heap, coder->program->data.scheduler);
	return (0);
}

t_coder	*heap_pop(t_heap *heap)
{
	t_coder	*tmp;
	int		i;

	if (!heap || !heap->arr)
		return (NULL);
	i = 0;
	tmp = heap->arr[i];
	heap->arr[i] = heap->arr[heap->size - 1];
	heap->size--;
	bubble_down(heap, heap->program->data.scheduler);
	return (tmp);
}

t_coder	*heap_top(t_heap *heap)
{
	if (!heap || heap->size == 0)
		return (NULL);
	return (heap->arr[0]);
}
