#include "vertex_priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>



static void ensure_extra_capacity(VertexPriorityQueue * pr)
{
    if(pr->size == pr->capacity)
    {
        pr->verticies = realloc(pr->verticies, pr->capacity * 2 * sizeof(*pr->verticies));
        pr->vertex_indexes = realloc(pr->vertex_indexes, pr->capacity * 2 * sizeof(*pr->vertex_indexes));
        pr->capacity *=2;
    }
}


static int get_left_child_index(int parent_index) {return 2*parent_index + 1;}
static int get_right_child_index(int parent_index) {return 2*parent_index + 2;}
static int get_parent_index(int childIndex) {return (childIndex-1)/2;}

static void swap_queued_vertex(VertexPriorityQueue * pr, int a_index, int b_index)
{
    QueuedVertex ** array = pr->verticies; 

    pr->vertex_indexes[pr->verticies[b_index]->index] = a_index;
    pr->vertex_indexes[pr->verticies[a_index]->index] = b_index;

    QueuedVertex * tmp = array[a_index];
    array[a_index] = array[b_index];
    array[b_index] = tmp;  

}


static void heapify_up(VertexPriorityQueue * pr, int in)
{
    int index = in;
    int parent_index = get_parent_index(index);

    while((parent_index >= 0) && (pr->verticies[parent_index]->dist > pr->verticies[index]->dist))
    {
        swap_queued_vertex(pr, index, parent_index);

        index = parent_index;
        parent_index = get_parent_index(index);
    }
} 

static void heapify_down(VertexPriorityQueue * pr, int in)
{
    int index = in;
    int left_child_index = get_left_child_index(index);
    int right_child_index = get_right_child_index(index);

    while(left_child_index < pr->size)
    {
        int smaller_child_index = left_child_index;
        if(right_child_index < pr->size && pr->verticies[right_child_index]->dist < pr->verticies[left_child_index]->dist)
            smaller_child_index = right_child_index;
        
        if(pr->verticies[index]->dist < pr->verticies[smaller_child_index]->dist)
            break;


        swap_queued_vertex(pr, index, smaller_child_index);

        index = smaller_child_index;
        left_child_index = get_left_child_index(index);
        right_child_index = get_right_child_index(index);
    }
} 

QueuedVertex * vertex_priority_queue_poll(VertexPriorityQueue * pr)
{
    if(pr->size == 0)
        return NULL;

    QueuedVertex * item = pr->verticies[0];

    pr->vertex_indexes[item->index] = -1;

    pr->verticies[0] = pr->verticies[pr->size -1];
    pr->size--;        

    heapify_down(pr, 0);

    return item;
}

void vertex_priority_queue_add(VertexPriorityQueue * pr, QueuedVertex * item)
{
    ensure_extra_capacity(pr);

    pr->verticies[pr->size] = item;
    pr->vertex_indexes[item->index] = pr->size;


    pr->size++;


    heapify_up(pr, pr->size-1);
}


VertexPriorityQueue * vertex_priority_queue_initalize(int init_capacity)
{
    VertexPriorityQueue * new_pr = malloc(sizeof(VertexPriorityQueue));


    new_pr->size = 0;
    new_pr->capacity = init_capacity;

    new_pr->verticies = malloc(new_pr->capacity * sizeof(*new_pr->verticies));
    new_pr->vertex_indexes = malloc(new_pr->capacity * sizeof(*new_pr->vertex_indexes));

    return new_pr;
}


void vertex_priority_queue_free(VertexPriorityQueue * pr)
{

    pr->size = 0;
    pr->capacity = 0;

    free(pr->vertex_indexes);
    free(pr->verticies);
    free(pr);
}


void vertex_priority_queue_update(VertexPriorityQueue * pr, int index, double new_dist)
{
    int index_of_updated_element = pr->vertex_indexes[index];

    if(new_dist > pr->verticies[index_of_updated_element]->dist)
    {
        pr->verticies[index_of_updated_element]->dist = new_dist;
        heapify_down(pr, index_of_updated_element);
    }
    else if(new_dist < pr->verticies[index_of_updated_element]->dist)
    {
        pr->verticies[index_of_updated_element]->dist = new_dist;
        heapify_up(pr, index_of_updated_element);
    }
}