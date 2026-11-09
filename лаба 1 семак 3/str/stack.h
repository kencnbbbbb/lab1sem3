#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "one_linked_list.h"
using namespace std;

struct Stack{
    OneLinkedList list; 
};
Stack Stack_create();
void Stack_destroy(Stack& stack);
void Stack_push_back(Stack& stack, string data);
string Stack_pop(Stack& stack);
string Stack_read(const Stack& stack);
#endif
