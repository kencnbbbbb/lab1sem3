#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include "node.h"
using namespace std;


struct LinkedList {
    Node* head;
    Node* tail;
};

LinkedList Linked_create();
void Linked_destroy(LinkedList& list);

void Linked_push_back(LinkedList& list, string data);
void Linked_push_front(LinkedList& list, string data);
void Linked_push_before(LinkedList& list, string next_el, string data);
void Linked_push_after(LinkedList& list, string prev_el, string data);

void Linked_pop_back(LinkedList& list);
void Linked_pop_front(LinkedList& list);
void Linked_pop_before(LinkedList& list, string next_el);
void Linked_pop_after(LinkedList& list, string prev_el);

void Linked_del_by_value(LinkedList& list, string value);
int Linked_find(LinkedList& list,string value);
string Linked_read(const LinkedList& list);
string Linked_read_reverse(const LinkedList& list);

#endif
