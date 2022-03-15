#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>

static void ensure_extra_capacity(PriorityQueue * pr)
{
    if(pr->size == pr->capacity)
    {
        pr->items = realloc(pr->items, pr->capacity * 2 * sizeof(*pr->items));
        pr->capacity *=2;
    }
}


static int get_left_child_index(int parent_index) {return 2*parent_index + 1;}
static int get_right_child_index(int parent_index) {return 2*parent_index + 2;}
static int get_parent_index(int childIndex) {return (childIndex-1)/2;}


static void heapify_up(PriorityQueue * pr, int in)
{
    int index = in;
    int parent_index = get_parent_index(index);

    while((parent_index >= 0) && (pr->items[parent_index] > pr->items[index]))
    {

        int tmp = pr->items[index];
        pr->items[index] = pr->items[parent_index];
        pr->items[parent_index] = tmp;

        index = parent_index;
        parent_index = get_parent_index(index);
    }
} 

static void heapify_down(PriorityQueue * pr, int in)
{
    int index = in;
    int left_child_index = get_left_child_index(index);
    int right_child_index = get_right_child_index(index);

    while(left_child_index < pr->size)
    {
        int smaller_child_index = left_child_index;
        if(right_child_index < pr->size && pr->items[right_child_index] < pr->items[left_child_index])
            smaller_child_index = right_child_index;
        
        if(pr->items[index] < pr->items[smaller_child_index])
            break;

        int tmp = pr->items[index];
        pr->items[index] = pr->items[smaller_child_index];
        pr->items[smaller_child_index] = tmp;

        index = smaller_child_index;
        left_child_index = get_left_child_index(index);
        right_child_index = get_right_child_index(index);
    }
} 

int priority_queue_poll(PriorityQueue * pr)
{
    if(pr->size == 0)
        return -1;

    int item = pr->items[0];
    pr->items[0] = pr->items[pr->size -1];
    pr->size--;
    heapify_down(pr, 0);

    return item;
}

int priority_queue_add(PriorityQueue * pr, int item)
{
    ensure_extra_capacity(pr);

    pr->items[pr->size] = item;
    pr->size++;
    heapify_up(pr, pr->size-1);

    return item;
}


PriorityQueue * priority_queue_initalize(int init_capacity)
{
    PriorityQueue * new_pr = malloc(sizeof(PriorityQueue));

    new_pr->size = 0;
    new_pr->capacity = init_capacity;
    new_pr->items = malloc(new_pr->capacity * sizeof(*new_pr->items));

    return new_pr;
}


void priority_queue_free(PriorityQueue * pr)
{
    free(pr->items);
    free(pr);
}