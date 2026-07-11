#include "codexion.h"

int heap_push(t_heap *heap, t_coder *coder)
{
    int i;
    t_coder *arr;

    i = 0;
    arr = heap->arr;
    if (!coder || !arr)
        return (1);
    arr[heap->size - 1] = *coder;
    heap->size += 1;
    
    return (0);
}

void    swap(t_coder *c1, t_coder *c2)
{
    t_coder     tmp;

    tmp = *c1;
    *c1 = *c2;
    *c2 = tmp;
}
