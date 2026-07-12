#include "codexion.h"

void    swap(t_coder **c1, t_coder **c2)
{
    t_coder     *tmp;

    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}

int has_priority(t_coder *a, t_coder *b, char *schedular)
{
    if (strcmp(schedular, "edf") == 0)
        return (a->deadline < b->deadline);
    return (a->arrival_time < b->arrival_time);
}

void    bubble_up(t_heap *heap, char *schedular)
{
    int i;
    int parent;

    i = heap->size - 1;
    parent = (i - 1) / 2;
    while (i != 0 || has_priority(heap->arr[i], heap->arr[parent], schedular))
    {
        swap(&heap->arr[i], &heap->arr[parent]);
        i = parent;
        parent = (i - 1) / 2;
    }
}

int heap_push(t_heap *heap, t_coder *coder)
{
    t_coder *arr;

    if (!heap || !coder || !heap->arr)
        return (1);
    if (heap->size >= heap->capacity)
    {
        fprintf(stderr,"Heap overflow");
        return (1);
    }
    heap->arr[heap->size] = coder;
    heap->size++;
    bubble_up(heap, coder->program->data.scheduler);
    return (0);
}



