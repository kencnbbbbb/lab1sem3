#include "linked_list.h"
#include <iostream>
#include <sstream>
using namespace std;

LinkedList Linked_create() {
    LinkedList list;
    list.head = nullptr;
    list.tail = nullptr;
    return list;
}

int Linked_find(LinkedList& list,string value){
    Node* node = list.head;
    int count = 0;
    while(node && node->data!= value){
        node = node->next;
        count ++;
    }
    if (!node){
        return -1;
    }
    return count;
}

void Linked_destroy(LinkedList& list) {
    while (list.head != nullptr) {
        Node* node = list.head;
        list.head = list.head->next;
        delete node;
    }
    list.tail = nullptr;
}

void Linked_push_back(LinkedList& list, string data) {
    Node* node = new Node{data, nullptr, list.tail};
    if (!list.head) {
        list.head = list.tail = node;
        return;
    }
    list.tail->next = node;
    list.tail = node;
}

void Linked_push_front(LinkedList& list, string data) {
    Node* node = new Node{data, list.head, nullptr};
    if (!list.head) {
        list.head = list.tail = node;
        return;
    }
    list.head->prev = node;
    list.head = node;
}

void Linked_push_before(LinkedList& list, string next_el, string data) {
    if (!list.head) return;
    if (next_el == list.head->data) {
        Linked_push_front(list, data);
        return;
    }

    Node* node = list.head;
    while(node && node->data != next_el){
        node = node->next;
    }
    if (!node){
        return;
    }
    Node* prev_el = node->prev;
    Node* new_node = new Node{data,nullptr,nullptr};
    new_node->next = node;
    new_node->prev = prev_el;
    prev_el->next = new_node;
    node->prev = new_node;
}

void Linked_push_after(LinkedList& list, string prev_el, string data) {

    if (!list.tail) return;


    Node* node = list.head;
    while (node && node->data != prev_el){
        node = node->next;
    }
    if (!node){
        return;
    }
    if (node == list.tail) { Linked_push_back(list, data); return; }
    Node* next_el = node->next;
    Node* new_node = new Node{data,nullptr,nullptr};
    new_node->next = next_el;
    new_node->prev = node;
    node->next = new_node;
    next_el -> prev = new_node;
}

void Linked_pop_back(LinkedList& list) {
    if (!list.head) return;

    if (list.head == list.tail) {
        delete list.head;
        list.head = list.tail = nullptr;
        return;
    }

    Node* temp = list.tail;
    list.tail = list.tail->prev;
    list.tail->next = nullptr;
    delete temp;
}

void Linked_pop_front(LinkedList& list) {
    if (!list.head) return;

    if (list.head == list.tail) {
        delete list.head;
        list.head = list.tail = nullptr;
        return;
    }

    Node* temp = list.head;
    list.head = list.head->next;
    list.head->prev = nullptr;
    delete temp;
}

void Linked_pop_before(LinkedList& list, string next_el) {

    Node* node = list.head;
    while (node && node->data != next_el) {
        node = node->next;
    }
    if (!node || node == list.head) {
        return;
    }

    Node* del_node = node->prev;

    if (del_node == list.head) {
        Linked_pop_front(list);
        return;
    }

    Node* prev_el = del_node->prev;
    prev_el->next = node;
    node->prev = prev_el;
    delete del_node;
}


void Linked_pop_after(LinkedList& list, string prev_el) {

    Node* node = list.head;
    while (node && node->data != prev_el) {
        node = node->next;
    }
    if (!node || node == list.tail) {
        return;
    }

    Node* del_node = node->next;

    if (del_node == list.tail) {
        Linked_pop_back(list);
        return;
    }

    Node* next_el = del_node->next;
    node->next = next_el;
    next_el->prev = node;
    delete del_node;
}


void Linked_del_by_value(LinkedList& list, string value) {
    if (!list.head) return;

    Node* node = list.head;
    while (node && node->data != value) node = node->next;
    if (!node) return;

    if (node == list.head) {
        Linked_pop_front(list);
        return;
    }
    if (node == list.tail) {
        Linked_pop_back(list);
        return;
    }

    Node* prev_el = node->prev;
    Node* next_el = node->next;
    prev_el->next = next_el;
    next_el->prev = prev_el;
    delete node;
}

string Linked_read(const LinkedList& list) {
    Node* node = list.head;
    stringstream ss;
    ss << " ";
    while (node) {
        ss << node->data << " ";
        node = node->next;
    }
    return ss.str();
}

string Linked_read_reverse(const LinkedList& list) {
    Node* node = list.tail;
    stringstream ss;
    ss << " ";
    while (node) {
        ss << node->data << " ";
        node = node->prev;
    }
    return ss.str();
}
