#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "one_linked_list.h"
using namespace std;

struct Queue{
    OneLinkedList list; 
};
Queue Queue_create();
void Queue_destroy(Queue& queue);
void Queue_push_back(Queue& queue, string data);
string Queue_pop(Queue& queue);
string Queue_read(const Queue& queue);
#endif
