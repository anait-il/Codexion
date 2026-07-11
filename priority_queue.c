#include "codexion.h"

void    swap(t_coder *c1, t_coder *c2)
{
    t_coder     tmp;

    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

int has_priority(t_waiter a, t_waiter b, char *schedular)
{
    if (strcmp(schedular, "edf") == 0)
        return (a.deadline < b.deadline);
    return (a.arrival_time < b.arrival_time);
}

void    bubble_up(t_heap *heap)
{
    int i;

    i = heap->size - 1;
    while (i != 0 && has_priority()
    {
        swap(&heap->arr[i], &heap->arr[(i - 1) / 2]);
        i--;
    }
}

int heap_push(t_heap *heap, t_coder *coder)
{
    int i;
    t_coder *arr;

    if (!coder || !arr)
        return (1);
    if (heap->size == heap->capacity)
    {
        fprintf(stderr,"Heap overflow");
        return (1);
    }
    i = 0;
    heap->arr[heap->size - 1] = *coder;
    heap->size++;
    bubble_up(heap);
    return (0);
}

