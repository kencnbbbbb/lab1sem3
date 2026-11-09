#include <iostream>
#include "stack.h"
#include "linked_list.h"

using namespace std;

Stack Stack_create(){
    Stack stack;
    stack.list = OneLinked_create();
    return stack;
}

void Stack_destroy(Stack& stack) {
    OneLinked_destroy(stack.list);
}

void Stack_push_back(Stack& stack, string data) {
    OneLinked_push_front(stack.list,data);
}

string Stack_pop(Stack& stack){
    if (!stack.list.head) throw runtime_error("Пустая структура");
    string value = stack.list.head->data;
    OneLinked_pop_front(stack.list);
    return value;
}

string Stack_read(const Stack& stack){
    return OneLinked_read(stack.list);
}
