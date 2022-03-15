#include <stdio.h>
#include <stdlib.h>

#include "priority_queue.h"


int main(int argc, char * argv[])
{
    PriorityQueue * pr = priority_queue_initalize(10);

    for(int i =10; i != 0; i--)
    {
        priority_queue_add(pr, i);
    }
    
    priority_queue_update(pr, 2, 1);
    
    priority_queue_update(pr, 5, 42);

    while(pr->size != 0)
    {
        printf("%d\n", priority_queue_poll(pr));
    }
   
    priority_queue_free(pr);

    return 0;
}
