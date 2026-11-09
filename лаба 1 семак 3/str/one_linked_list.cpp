#include <iostream>
#include <vector>
#include "one_linked_list.h"
#include <sstream>
using namespace std;

OneLinkedList OneLinked_create() {
    OneLinkedList list;
    list.head = nullptr;
    list.tail = nullptr;
    return list;
}

void OneLinked_destroy(OneLinkedList& list) {
    while (list.head != nullptr) {
        OneNode* node = list.head;
        list.head = list.head->next;
        delete node;
    }
    list.tail = nullptr;
}

void OneLinked_pop_front(OneLinkedList& list) {
    if (!list.head) return;
    OneNode* node = list.head;
    list.head = list.head->next;
    if (!list.head) list.tail = nullptr;
    delete node;
}

void OneLinked_pop_back(OneLinkedList& list) {
    if (!list.head) return;
    if (list.head == list.tail) {
        delete list.head;
        list.head = list.tail = nullptr;
        return;
    }
    OneNode* node = list.head;
    while (node->next != list.tail) node = node->next;
    delete list.tail;
    list.tail = node;
    list.tail->next = nullptr;
}

void OneLinked_pop_after(OneLinkedList& list, string prev_value) {
    if (!list.head) return;

    OneNode* prev = list.head;
    while (prev && prev->data != prev_value) prev = prev->next;
    if (!prev || !prev->next) return;

    OneNode* del_el = prev->next;
    prev->next = del_el->next;
    if (del_el == list.tail) list.tail = prev;
    delete del_el;
}

void OneLinked_pop_before(OneLinkedList& list, string next_value) {
    if (!list.head) return;

    if (list.head->data == next_value) return;

    if (list.head->next && list.head->next->data == next_value) {
        OneLinked_pop_front(list);
        return;
    }

    OneNode* prev = list.head;
    while (prev && prev->next && prev->next->next && prev->next->next->data != next_value) {
        prev = prev->next;
    }
    if (!prev || !prev->next || !prev->next->next) return;

    OneNode* del_el = prev->next;
    prev->next = del_el->next;
    delete del_el;
}

void OneLinked_push_back(OneLinkedList& list, string data) {
    OneNode* node = new OneNode{data, nullptr};
    if (!list.head) {
        list.head = list.tail = node;
    } else {
        list.tail->next = node;
        list.tail = node;
    }
}

void OneLinked_push_front(OneLinkedList& list, string data) {
    OneNode* node = new OneNode{data, nullptr};
    if (!list.head) {
        list.head = list.tail = node;
    } else {
        node->next = list.head;
        list.head = node;
    }
}

void OneLinked_push_after(OneLinkedList& list, string prev_value, string data) {
    if (!list.head) return;

    OneNode* prev = list.head;
    while (prev && prev->data != prev_value) prev = prev->next;
    if (!prev) return;

    OneNode* node = new OneNode{data, prev->next};
    prev->next = node;
    if (prev == list.tail) list.tail = node;
}

void OneLinked_push_before(OneLinkedList& list, string next_value, string data) {
    if (!list.head) return;

    if (list.head->data == next_value) {
        OneLinked_push_front(list, data);
        return;
    }

    OneNode* prev = list.head;
    while (prev && prev->next && prev->next->data != next_value) prev = prev->next;
    if (!prev || !prev->next) return;

    OneNode* node = new OneNode{data, prev->next};
    prev->next = node;
}

void OneLinked_del_by_value(OneLinkedList& list, string value) {
    if (!list.head) return;
    if (list.head->data == value) {
        OneLinked_pop_front(list);
        return;
    }
    OneNode* node = list.head;
    while (node->next && node->next->data != value) node = node->next;
    if (node->next) {
        OneNode* temp = node->next;
        node->next = temp->next;
        if (temp == list.tail) list.tail = node;
        delete temp;
    }
}

int OneLinked_find_by_value(OneLinkedList& list, string value){
    if (!list.head) return -1;
    if (list.head->data == value){
        return 0;
    }
    int count = 0;
    OneNode* node = list.head;
    while (node && node->data != value){
        node = node->next;
        count ++;
    }
    if (node){
        return count;
    }
    return -1;
}

string OneLinked_read(const OneLinkedList& list){
    if (!list.head){
        return "";
    }

    stringstream ss;
    OneNode* node = list.head;
    while (node != list.tail){
        ss << " ";
        ss << node->data;
        node = node -> next;
    }
    if (node == list.tail){
        ss << " ";
    }
    ss << node->data;
    return ss.str();
}

string OneLinked_read_reverse(const OneLinkedList& list){
    if (!list.head){
        return "";
    }

    stringstream ss;
    OneNode* node = list.head;
    vector<string> temp;
    while (node != list.tail){
        temp.push_back(node->data);
        node = node -> next;
    }

    temp.push_back(node->data);
    for (int i = static_cast<int>(temp.size()) - 1; i >= 0; --i){
        ss << " ";
        ss << temp[i];
    }
    return ss.str();
}