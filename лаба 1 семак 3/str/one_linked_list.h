#ifndef ONE_LINKED_LIST_H
#define ONE_LINKED_LIST_H

#include <iostream>

using namespace std;

struct OneNode {
    string data;
    OneNode* next;
};

struct OneLinkedList {
    OneNode* head;
    OneNode* tail;
};

OneLinkedList OneLinked_create();
void OneLinked_destroy(OneLinkedList& list);

void OneLinked_pop_front(OneLinkedList& list);
void OneLinked_pop_back(OneLinkedList& list);
void OneLinked_pop_after(OneLinkedList& list, string prev_el);
void OneLinked_pop_before(OneLinkedList& list, string next_el);

void OneLinked_push_back(OneLinkedList& list, string data);
void OneLinked_push_front(OneLinkedList& list, string data);
void OneLinked_push_after(OneLinkedList& list, string prev, string data);
void OneLinked_push_before(OneLinkedList& list, string next_el, string data);

void OneLinked_del_by_value(OneLinkedList& list, string value);
int OneLinked_find_by_value(OneLinkedList& list, string value);
string OneLinked_read(const OneLinkedList& list);
string OneLinked_read_reverse(const OneLinkedList& list);

#endif
