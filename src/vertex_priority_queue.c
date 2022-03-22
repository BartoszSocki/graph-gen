#include "vertex_priority_queue.h"
#include <stdio.h> // stderr
#include <stdlib.h> // malloc, realloc
#include <float.h> //DBL_MAX

#define GET_LEFT_CHILD_INDEX(pi) 2*pi+1
#define GET_RIGHT_CHILD_INDEX(pi) 2*pi+2
#define GET_PARENT_INDEX(ci) (ci-1)/2 

#define ALLOW_RESIZEING_QUEUE 0 
#if ALLOW_RESIZEING_QUEUE 
static void ensure_extra_capacity(VertexPriorityQueue * pr)
{
    if(pr->size == pr->capacity)
    {
        QueuedVertex ** new_verticies_pointer = NULL;
        int * new_vertex_index_pointer = NULL;


        new_verticies_pointer =  realloc(pr->verticies, pr->capacity * 2 * sizeof(*pr->verticies));
        new_vertex_index_pointer = realloc(pr->verticies_indexes, pr->capacity * 2 * sizeof(*pr->verticies_indexes));

        if((new_verticies_pointer == NULL) || (new_vertex_index_pointer == NULL))
        {
            fprintf(stderr, "ERROR: can't reallocate elements.\n");
        }
        else
        {
            pr->verticies = new_verticies_pointer;
            pr->verticies_indexes = new_vertex_index_pointer;
        }

        pr->capacity *=2;
    }
}
#endif

static void swap_queued_vertex(VertexPriorityQueue * pr, int a_index, int b_index)
{
    QueuedVertex ** array = pr->verticies; 

    //update stored indexes
    pr->verticies_indexes[pr->verticies[b_index]->index] = a_index;
    pr->verticies_indexes[pr->verticies[a_index]->index] = b_index;

    //swap elements in the array 
    QueuedVertex * tmp = array[a_index];
    array[a_index] = array[b_index];
    array[b_index] = tmp;  

}


static void heapify_up(VertexPriorityQueue * pr, int in)
{
    int index = in;
    int parent_index = GET_PARENT_INDEX(index);

    //move up the tree and swap elements until the order is restored
    while((parent_index >= 0) && (pr->verticies[parent_index]->dist > pr->verticies[index]->dist))
    {
        swap_queued_vertex(pr, index, parent_index);

        index = parent_index;
        parent_index = GET_PARENT_INDEX(index);
    }
} 

static void heapify_down(VertexPriorityQueue * pr, int in)
{
    int index = in;
    int left_child_index = GET_LEFT_CHILD_INDEX(index);
    int right_child_index = GET_RIGHT_CHILD_INDEX(index);

    //move down the tree and swap elements until the order is restored
    while(left_child_index < pr->size)
    {
        int smaller_child_index = left_child_index;
        if(right_child_index < pr->size && pr->verticies[right_child_index]->dist < pr->verticies[left_child_index]->dist)
            smaller_child_index = right_child_index;
        
        if(pr->verticies[index]->dist < pr->verticies[smaller_child_index]->dist)
            break;


        swap_queued_vertex(pr, index, smaller_child_index);

        index = smaller_child_index;
        left_child_index = GET_LEFT_CHILD_INDEX(index);
        right_child_index = GET_RIGHT_CHILD_INDEX(index);
    }
} 

QueuedVertex * vertex_priority_queue_poll(VertexPriorityQueue * pr)
{
    if(pr->size == 0)
        return NULL;

    QueuedVertex * item = pr->verticies[0];
    pr->verticies_indexes[item->index] = -1;

    //swap the root element with the element on the end of the queue
    pr->verticies[0] = pr->verticies[pr->size -1];
    pr->size--;        

    //restore the order
    heapify_down(pr, 0);

    return item;
}

void vertex_priority_queue_add(VertexPriorityQueue * pr, QueuedVertex * item)
{
    #if ALLOW_RESIZEING_QUEUE 
    ensure_extra_capacity(pr);
    #endif

    //add element to the end
    pr->verticies[pr->size] = item;
    pr->verticies_indexes[item->index] = pr->size;
    pr->size++;

    //restore the order of the heap
    heapify_up(pr, pr->size-1);
}


VertexPriorityQueue * vertex_priority_queue_initalize(int init_capacity)
{
    VertexPriorityQueue * new_pr = malloc(sizeof(VertexPriorityQueue));


    new_pr->size = 0;
    new_pr->capacity = init_capacity;

    new_pr->verticies = malloc(new_pr->capacity * sizeof(*new_pr->verticies));
    new_pr->verticies_indexes = malloc(new_pr->capacity * sizeof(*new_pr->verticies_indexes));

    return new_pr;
}


void vertex_priority_queue_free(VertexPriorityQueue * pr)
{
    pr->size = 0;
    pr->capacity = 0;

    free(pr->verticies_indexes);
    free(pr->verticies);
    free(pr);

}


void vertex_priority_queue_update(VertexPriorityQueue * pr, int index, double new_dist)
{
    int index_of_element_to_be_updated = pr->verticies_indexes[index];

    //check whether the dist value has increased or decreased and restore the order of heap
    if(new_dist > pr->verticies[index_of_element_to_be_updated]->dist)
    {
        pr->verticies[index_of_element_to_be_updated]->dist = new_dist;
        heapify_down(pr, index_of_element_to_be_updated);
    }
    else if(new_dist < pr->verticies[index_of_element_to_be_updated]->dist)
    {
        pr->verticies[index_of_element_to_be_updated]->dist = new_dist;
        heapify_up(pr, index_of_element_to_be_updated);
    }
}